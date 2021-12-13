#include "BOL/game/terrainBrush.h"

IMPLEMENT_CO_NETOBJECT_V1(TerrainBrush);

ConsoleDocClass( TerrainBrush,
     "@brief A TerrainBrush allows script access to terrain data and networked deformation.\n\n"
);

void TerrainBrush::initPersistFields()
{
    Parent::initPersistFields();
}

U32 TerrainBrush::packUpdate(NetConnection *conn, U32 mask, BitStream* stream)
{
    return Parent::packUpdate(conn, mask, stream);
}

void TerrainBrush::unpackUpdate(NetConnection *conn, BitStream* stream)
{
    Parent::unpackUpdate(conn, stream);
}

void TerrainBrush::getGridPoints(Vector<GridPointSet>& out)
{
    Con::errorf("::getGridPoints not implemented!");
}

void TerrainBrush::getCandidateTerrains(Vector<TerrainBlock*>& out)
{
    // FIXME: Networking
    SceneContainer* container = &gServerContainer;
   // if (isGhost())
   // {
        container = &gClientContainer;
   // }

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

F32 TerrainBrush::getAverageTerrainHeight()
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(gridPointSets);

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

F32 TerrainBrush::getMinimumTerrainHeight()
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(gridPointSets);

    F32 minimumHeight = F32_MAX;
    for (U32 setIteration = 0; setIteration < gridPointSets.size(); ++setIteration)
    {
        const GridPointSet &pointSet = gridPointSets[setIteration];

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

F32 TerrainBrush::getMaximumTerrainHeight()
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(gridPointSets);

    F32 maximumHeight = F32_MIN;
    for (U32 setIteration = 0; setIteration < gridPointSets.size(); ++setIteration)
    {
        const GridPointSet &pointSet = gridPointSets[setIteration];

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

void TerrainBrush::lower(F32 height)
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(gridPointSets);

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
            if(centerPosition.x < gridUpdateMin.x)
                gridUpdateMin.x = centerPosition.x;
            if(centerPosition.y < gridUpdateMin.y)
                gridUpdateMin.y = centerPosition.y;
            if(centerPosition.x > gridUpdateMax.x)
                gridUpdateMax.x = centerPosition.x;
            if(centerPosition.y > gridUpdateMax.y)
                gridUpdateMax.y = centerPosition.y;

            currentSet.mTerrain->setHeight(centerPosition, currentHeight - height);
        }

        //if(gridUpdateMin.x <= gridUpdateMax.x)
        currentSet.mTerrain->updateGrid(gridUpdateMin, gridUpdateMax);
    }
}

void TerrainBrush::flatten()
{
    Vector<GridPointSet> gridPointSets;
    getGridPoints(gridPointSets);

    const F32 averageHeight = getAverageTerrainHeight();

    for (U32 pointSetIteration = 0; pointSetIteration < gridPointSets.size(); ++pointSetIteration)
    {
        const GridPointSet &currentSet = gridPointSets[pointSetIteration];

        // Keep track of the min and max update
        Point2I gridUpdateMin(U32_MAX, U32_MAX);
        Point2I gridUpdateMax(0, 0);

        for (U32 pointIteration = 0; pointIteration < currentSet.mGridPoints.size(); ++pointIteration)
        {
            const Point2I& currentPoint = currentSet.mGridPoints[pointIteration];

            // Calculate center position
            Point2I centerPosition;
            currentSet.mTerrain->gridToCenter(currentPoint, centerPosition);
            if(centerPosition.x < gridUpdateMin.x)
                gridUpdateMin.x = centerPosition.x;
            if(centerPosition.y < gridUpdateMin.y)
                gridUpdateMin.y = centerPosition.y;
            if(centerPosition.x > gridUpdateMax.x)
                gridUpdateMax.x = centerPosition.x;
            if(centerPosition.y > gridUpdateMax.y)
                gridUpdateMax.y = centerPosition.y;

            currentSet.mTerrain->setHeight(centerPosition, averageHeight);
        }

        //if(gridUpdateMin.x <= gridUpdateMax.x)
        currentSet.mTerrain->updateGrid(gridUpdateMin, gridUpdateMax);
    }
}

void TerrainBrush::paintNoise(F32 heightVariation, S32 seed)
{
    MRandomLCG generator;
    generator.setSeed(seed);

    Vector<GridPointSet> gridPointSets;
    getGridPoints(gridPointSets);

    for (U32 pointSetIteration = 0; pointSetIteration < gridPointSets.size(); ++pointSetIteration)
    {
        const GridPointSet &currentSet = gridPointSets[pointSetIteration];

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
            if(centerPosition.x < gridUpdateMin.x)
                gridUpdateMin.x = centerPosition.x;
            if(centerPosition.y < gridUpdateMin.y)
                gridUpdateMin.y = centerPosition.y;
            if(centerPosition.x > gridUpdateMax.x)
                gridUpdateMax.x = centerPosition.x;
            if(centerPosition.y > gridUpdateMax.y)
                gridUpdateMax.y = centerPosition.y;

            // Retrieve next random
            const F32 randomDelta = generator.randF(-heightVariation, heightVariation);
            currentSet.mTerrain->setHeight(centerPosition, currentHeight - randomDelta);
        }

        //if(gridUpdateMin.x <= gridUpdateMax.x)
        currentSet.mTerrain->updateGrid(gridUpdateMin, gridUpdateMax);
    }
}

DefineEngineMethod( TerrainBrush, getMaximumHeight, F32, ( ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@returns The average terrain height at the world position.\n")
{
    return object->getMaximumTerrainHeight();
}

DefineEngineMethod( TerrainBrush, getMinimumHeight, F32, ( ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@returns The average terrain height at the world position.\n")
{
    return object->getMinimumTerrainHeight();
}

DefineEngineMethod( TerrainBrush, getAverageHeight, F32, ( ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@returns The average terrain height at the world position.\n")
{
    return object->getAverageTerrainHeight();
}

DefineEngineMethod( TerrainBrush, lower, void, ( F32 height ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@param height The height to lower by.\n"
                    "@returns The average terrain height at the world position.\n")
{
    object->lower(height);
}

DefineEngineMethod( TerrainBrush, flatten, void, ( ),,
                    "@brief Flatten the terrain at the specified position.")
{
    object->flatten();
}

DefineEngineMethod( TerrainBrush, paintNoise, void, ( F32 heightVariation, S32 seed ),,
                    "@brief Flatten the terrain at the specified position.")
{
    object->paintNoise(heightVariation, seed);
}

//--------------------------------------------------------------------------

IMPLEMENT_CO_NETOBJECT_V1(RectangleTerrainBrush);

ConsoleDocClass( RectangleTerrainBrush,
                 "@brief A TerrainBrush allows script access to terrain data and networked deformation.\n\n"
);

void RectangleTerrainBrush::getGridPoints(Vector<GridPointSet>& out)
{
    const F32 halfWidth = mWidth / 2.0f;
    const F32 halfHeight = mHeight / 2.0f;

    // Once we know the half dimensions, calculate a box centered around our world position
    const Point3F worldPosition = getPosition();

    const Point3F topLeft = Point3F(worldPosition.x - halfWidth, worldPosition.y - halfHeight, 0.0f);
    const Point3F bottomRight = Point3F(worldPosition.x + halfWidth, worldPosition.y + halfHeight, 0.0f);

    // Find affected terrains
    Vector<TerrainBlock*> candidateTerrains;
    getCandidateTerrains(candidateTerrains);

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

U32 RectangleTerrainBrush::packUpdate(NetConnection *conn, U32 mask, BitStream* stream)
{
    stream->writeFloat(mWidth, 10);
    stream->writeFloat(mHeight, 10);
    return Parent::packUpdate(conn, mask, stream);
}

void RectangleTerrainBrush::unpackUpdate(NetConnection *conn, BitStream* stream)
{
    mWidth = stream->readFloat(10);
    mHeight = stream->readFloat(10);
    Parent::unpackUpdate(conn, stream);
}

void RectangleTerrainBrush::initPersistFields()
{
    addField("width", TypeF32, Offset(mWidth, RectangleTerrainBrush), "The width of the brush.");
    addField("height", TypeF32, Offset(mHeight, RectangleTerrainBrush), "The height of the brush.");

    Parent::initPersistFields();
}