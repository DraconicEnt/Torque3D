#include "BOL/game/deformationManager.h"

IMPLEMENT_CO_NETOBJECT_V1(DeformationManager);

ConsoleDocClass(DeformationManager,
     "@brief A TerrainBrush allows script access to terrain data and networked deformation.\n\n"
);

DeformationManager::DeformationManager()
{
    mNetFlags.set(ScopeAlways | Ghostable);
}

void DeformationManager::initPersistFields()
{
    Parent::initPersistFields();
}

U32 DeformationManager::packUpdate(NetConnection* conn, U32 mask, BitStream* stream)
{
    // Determine the starting index to network for this connection
    U32 startIndex = 0;

    auto search = mNetConnectionState.find(conn);
    if (search != mNetConnectionState.end())
    {
        startIndex = (*search).value;
    }

    if (startIndex < mActions.size())
    {
        const U32 packedActionCount = mActions.size() - startIndex;
        stream->write(packedActionCount);

        for (U32 iteration = startIndex; iteration < mActions.size(); ++iteration)
        {
            DeformationAction* action = mActions[iteration];
            action->packData(stream);
        }

        mNetConnectionState[conn] = mActions.size();
    }
    else
    {
        // Zero actions to pack
        stream->write((U32)0);
    }

    return Parent::packUpdate(conn, mask, stream);
}

void DeformationManager::unpackUpdate(NetConnection* conn, BitStream* stream)
{
    U32 packedActionCount;
    stream->read(&packedActionCount);

    for (U32 iteration = 0; iteration < packedActionCount; ++iteration)
    {
        DeformationAction* newAction = DeformationAction::unpackData(stream);
        AssertFatal(newAction, "Failed to unpack deformation action!");

        newAction->applyAction(false);
        mActions.push_back(newAction);
    }
    Parent::unpackUpdate(conn, stream);
}

DeformationManager* DeformationManager::getDeformationManager()
{
    DeformationManager* manager;
    if (!Sim::findObject<DeformationManager>("DeformationManagerInstance", manager))
    {
        manager = new DeformationManager();
        manager->assignName("DeformationManagerInstance");
        manager->registerObject();

        // FIXME: Add to mission cleanup?
        SimGroup* missionCleanup;
        if (Sim::findObject<SimGroup>("MissionCleanup", missionCleanup))
        {
            missionCleanup->addObject(manager);
        }
    }

    return manager;
}

void DeformationManager::registerAction(DeformationAction* action)
{
    // Execute action server side immediately
    action->applyAction(true);

    // Store for networking
    mActions.push_back(action);

    // FIXME: Define mask bit for this
    setMaskBits(1);
}

//--------------------------------------------------------------------------

static void getCandidateTerrains(bool isServer, VectorPtr<TerrainBlock*>& out)
{
    SceneContainer* container = &gServerContainer;
    if (!isServer)
    {
        container = &gClientContainer;
    }

    Vector<SceneObject*> foundTerrains;
    container->findObjectList(TerrainObjectType, &foundTerrains);

    out.reserve(foundTerrains.size());
    for (U32 iteration = 0; iteration < foundTerrains.size(); ++iteration)
    {
        TerrainBlock* currentBlock = dynamic_cast<TerrainBlock*>(foundTerrains[iteration]);

        AssertFatal(currentBlock, "Expected TerrainBlock instances in call to findObjectList!");
        out.push_back(currentBlock);
    }
}

//--------------------------------------------------------------------------

void DeformationBrush::packData(BitStream* stream)
{
    stream->writeCompressedPoint(mPosition);
}

DeformationBrush* DeformationBrush::unpackData(BitStream* stream)
{
    U8 brushID;
    stream->read(&brushID);

    switch (brushID)
    {
        case DeformationBrushIDs::RectangleBrushID:
            return RectangleDeformationBrush::unpackData(stream);
    }

    Con::errorf("Unknown brush ID in unpack!");
    return NULL;
}

F32 DeformationBrush::getAverageTerrainHeight(bool isServer)
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(isServer, gridPointSets);

    if (gridPointSets.size() == 0)
    {
        return 0.0f;
    }

    // Enumerate sets
    U32 pointCount = 0;
    F32 averageHeight = 0.0f;
    for (U32 setIteration = 0; setIteration < gridPointSets.size(); ++setIteration)
    {
        const GridPointSet& pointSet = gridPointSets[setIteration];

        pointCount += pointSet.mGridPoints.size();
        for (U32 pointIteration = 0; pointIteration < pointSet.mGridPoints.size(); ++pointIteration)
        {
            const Point2I& point = pointSet.mGridPoints[pointIteration];

            AssertFatal(pointSet.mTerrain->isPointInTerrain(point), "Expected points to be on terrain!");
            const F32 pointHeight = pointSet.mTerrain->getGridHeight(point);

            averageHeight += pointHeight;
        }
    }

    if (pointCount == 0)
    {
        return 0.0f;
    }

    return averageHeight / pointCount;
}

F32 DeformationBrush::getMinimumTerrainHeight(bool isServer)
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(isServer, gridPointSets);

    F32 minimumHeight = F32_MAX;
    for (U32 setIteration = 0; setIteration < gridPointSets.size(); ++setIteration)
    {
        const GridPointSet& pointSet = gridPointSets[setIteration];

        for (U32 pointIteration = 0; pointIteration < pointSet.mGridPoints.size(); ++pointIteration)
        {
            const Point2I& point = pointSet.mGridPoints[pointIteration];

            AssertFatal(pointSet.mTerrain->isPointInTerrain(point), "Expected points to be on terrain!");
            const F32 pointHeight = pointSet.mTerrain->getGridHeight(point);

            if (pointHeight < minimumHeight)
            {
                minimumHeight = pointHeight;
            }
        }
    }

    return minimumHeight;
}

F32 DeformationBrush::getMaximumTerrainHeight(bool isServer)
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(isServer, gridPointSets);

    F32 maximumHeight = F32_MIN;
    for (U32 setIteration = 0; setIteration < gridPointSets.size(); ++setIteration)
    {
        const GridPointSet& pointSet = gridPointSets[setIteration];

        for (U32 pointIteration = 0; pointIteration < pointSet.mGridPoints.size(); ++pointIteration)
        {
            const Point2I& point = pointSet.mGridPoints[pointIteration];

            AssertFatal(pointSet.mTerrain->isPointInTerrain(point), "Expected points to be on terrain!");
            const F32 pointHeight = pointSet.mTerrain->getGridHeight(point);

            if (pointHeight > maximumHeight)
            {
                maximumHeight = pointHeight;
            }
        }
    }

    return maximumHeight;
}

//--------------------------------------------------------------------------

RectangleDeformationBrush::RectangleDeformationBrush() : mWidth(10), mHeight(10)
{

}

RectangleDeformationBrush::RectangleDeformationBrush(const RectangleDeformationBrush& copied) : mWidth(copied.mWidth), mHeight(copied.mHeight)
{

}

RectangleDeformationBrush::RectangleDeformationBrush(F32 width, F32 height) : mWidth(width), mHeight(height)
{

}

void RectangleDeformationBrush::packData(BitStream* stream)
{
    stream->write((U8)DeformationBrushIDs::RectangleBrushID);

    stream->write(mWidth);
    stream->write(mHeight);
    Parent::packData(stream);
}

RectangleDeformationBrush* RectangleDeformationBrush::unpackData(BitStream* stream)
{
    F32 width;
    F32 height;

    stream->read(&width);
    stream->read(&height);
    return new RectangleDeformationBrush(width, height);
}

void RectangleDeformationBrush::getGridPoints(bool isServer, Vector<GridPointSet>& out)
{
    const F32 halfWidth = mWidth / 2.0f;
    const F32 halfHeight = mHeight / 2.0f;

    const Point3F topLeft = Point3F(mPosition.x - halfWidth, mPosition.y - halfHeight, 0.0f);
    const Point3F bottomRight = Point3F(mPosition.x + halfWidth, mPosition.y + halfHeight, 0.0f);

    // Find affected terrains
    VectorPtr<TerrainBlock*> candidateTerrains;
    getCandidateTerrains(isServer, candidateTerrains);

    for (U32 iteration = 0; iteration < candidateTerrains.size(); ++iteration)
    {
        Point2I gridTopLeft;
        Point2I gridBottomRight;

        TerrainBlock* currentBlock = candidateTerrains[iteration];

        // FIXME: Handle terrain block crossover
        if (currentBlock->worldToGrid(topLeft, gridTopLeft) && currentBlock->worldToGrid(bottomRight, gridBottomRight))
        {
            // Terrain block is affected, calculate affected points
            Point2I currentGridPosition;
            GridPointSet newSet;
            newSet.mTerrain = currentBlock;

            for (currentGridPosition.x = gridTopLeft.x; currentGridPosition.x <= gridBottomRight.x; ++currentGridPosition.x)
            {
                for (currentGridPosition.y = gridTopLeft.y; currentGridPosition.y <= gridBottomRight.y; ++currentGridPosition.y)
                {
                    // Make sure the point is valid
                    if (currentBlock->isPointInTerrain(currentGridPosition))
                    {
                        newSet.mGridPoints.push_back(currentGridPosition);
                    }
                }
            }

            out.push_back(newSet);
        }
    }
}

//--------------------------------------------------------------------------

DeformationAction::DeformationAction(DeformationBrush* brush) : mBrush(brush)
{

}

DeformationAction* DeformationAction::unpackData(BitStream* stream)
{
    U8 actionID;
    stream->read(&actionID);

    switch (actionID)
    {
        case DeformationActionIDs::FlattenActionID:
            return FlattenDeformationAction::unpackData(stream);
        case DeformationActionIDs::AdjustHeightActionID:
            return AdjustHeightDeformationAction::unpackData(stream);
    }

    Con::errorf("Unknown action ID in unpack!");
    return NULL;
}

//--------------------------------------------------------------------------

FlattenDeformationAction::FlattenDeformationAction(DeformationBrush* brush) : DeformationAction(brush)
{

}

void FlattenDeformationAction::packData(BitStream* stream)
{
    stream->write((U8)DeformationActionIDs::FlattenActionID);
    mBrush->packData(stream);
}

FlattenDeformationAction* FlattenDeformationAction::unpackData(BitStream* stream)
{
    DeformationBrush* brush = DeformationBrush::unpackData(stream);
    AssertFatal(brush, "Expected valid brush in unpack!");

    return new FlattenDeformationAction(brush);
}

void FlattenDeformationAction::applyAction(bool isServer)
{
    Vector<GridPointSet> gridPointSets;
    mBrush->getGridPoints(isServer, gridPointSets);

    const F32 averageHeight = mBrush->getAverageTerrainHeight(isServer);

    for (U32 pointSetIteration = 0; pointSetIteration < gridPointSets.size(); ++pointSetIteration)
    {
        const GridPointSet& currentSet = gridPointSets[pointSetIteration];

        // Keep track of the min and max update
        Point2I gridUpdateMin(U32_MAX, U32_MAX);
        Point2I gridUpdateMax(0, 0);

        for (U32 pointIteration = 0; pointIteration < currentSet.mGridPoints.size(); ++pointIteration)
        {
            const Point2I& currentPoint = currentSet.mGridPoints[pointIteration];

            // Calculate center position
            Point2I centerPosition;
            currentSet.mTerrain->gridToCenter(currentPoint, centerPosition);
            if (centerPosition.x < gridUpdateMin.x)
                gridUpdateMin.x = centerPosition.x;
            if (centerPosition.y < gridUpdateMin.y)
                gridUpdateMin.y = centerPosition.y;
            if (centerPosition.x > gridUpdateMax.x)
                gridUpdateMax.x = centerPosition.x;
            if (centerPosition.y > gridUpdateMax.y)
                gridUpdateMax.y = centerPosition.y;

            currentSet.mTerrain->setHeight(centerPosition, averageHeight);
        }

        //if(gridUpdateMin.x <= gridUpdateMax.x)
        currentSet.mTerrain->updateGrid(gridUpdateMin, gridUpdateMax);
    }
}

//--------------------------------------------------------------------------

AdjustHeightDeformationAction::AdjustHeightDeformationAction(F32 height, DeformationBrush* brush) : mHeight(height), DeformationAction(brush)
{

}

void AdjustHeightDeformationAction::packData(BitStream* stream)
{
    stream->write((U8)DeformationActionIDs::AdjustHeightActionID);
    stream->write(mHeight);
    mBrush->packData(stream);
}

AdjustHeightDeformationAction* AdjustHeightDeformationAction::unpackData(BitStream* stream)
{
    F32 height;
    stream->read(&height);

    DeformationBrush* brush = DeformationBrush::unpackData(stream);
    AssertFatal(brush, "Expected valid brush in unpack!");

    return new AdjustHeightDeformationAction(height, brush);
}

void AdjustHeightDeformationAction::applyAction(bool isServer)
{
    Vector<GridPointSet> gridPointSets;
    mBrush->getGridPoints(isServer, gridPointSets);

    for (U32 pointSetIteration = 0; pointSetIteration < gridPointSets.size(); ++pointSetIteration)
    {
        const GridPointSet& currentSet = gridPointSets[pointSetIteration];

        // Keep track of the min and max update
        Point2I gridUpdateMin(U32_MAX, U32_MAX);
        Point2I gridUpdateMax(0, 0);

        for (U32 pointIteration = 0; pointIteration < currentSet.mGridPoints.size(); ++pointIteration)
        {
            const Point2I& currentPoint = currentSet.mGridPoints[pointIteration];
            const F32 currentHeight = currentSet.mTerrain->getGridHeight(currentPoint);

            // Calculate center position
            Point2I centerPosition;
            currentSet.mTerrain->gridToCenter(currentPoint, centerPosition);
            if (centerPosition.x < gridUpdateMin.x)
                gridUpdateMin.x = centerPosition.x;
            if (centerPosition.y < gridUpdateMin.y)
                gridUpdateMin.y = centerPosition.y;
            if (centerPosition.x > gridUpdateMax.x)
                gridUpdateMax.x = centerPosition.x;
            if (centerPosition.y > gridUpdateMax.y)
                gridUpdateMax.y = centerPosition.y;

            currentSet.mTerrain->setHeight(centerPosition, currentHeight + mHeight);
        }

        //if(gridUpdateMin.x <= gridUpdateMax.x)
        currentSet.mTerrain->updateGrid(gridUpdateMin, gridUpdateMax);
    }
}
