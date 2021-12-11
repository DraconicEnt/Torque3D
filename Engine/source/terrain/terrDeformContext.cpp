#include "terrain/terrDeformContext.h"

#ifndef _TERRAINEDITOR_H_
#include "gui/worldEditor/terrainEditor.h"
#endif

TerrainDeformContext::TerrainDeformContext(TerrainEditor* editor)
{
    mNeedsGridUpdate = false;
    mNeedsMaterialUpdate = false;
    mTerrain = NULL;
    mSelection = new Selection();
    mGridUpdateMin = Point2I(S32_MAX, S32_MAX);
    mGridUpdateMax = Point2I(0, 0);
    mUndoSelection = new Selection();
}

TerrainDeformContext::~TerrainDeformContext()
{
    delete mSelection;
    delete mUndoSelection;
}

void TerrainDeformContext::setGridMaterial( const Point2I &gPoint, U8 index )
{
    PROFILE_SCOPE( TerrainEditor_SetGridMaterial );

    Point2I cPos;
    mTerrain->gridToCenter( gPoint, cPos );
    TerrainFile *file = mTerrain->getFile();

    // If we changed the empty state then we need
    // to do a grid update as well.
    U8 currIndex = file->getLayerIndex( cPos.x, cPos.y );
    if (  ( currIndex == (U8)-1 && index != (U8)-1 ) ||
          ( currIndex != (U8)-1 && index == (U8)-1 ) )
    {
        mGridUpdateMin.setMin( cPos );
        mGridUpdateMax.setMax( cPos );
        mNeedsGridUpdate = true;
    }

    file->setLayerIndex( cPos.x, cPos.y, index );
}

void TerrainDeformContext::setGridHeight(const Point2I & gPoint, const F32 height)
{
    PROFILE_SCOPE( TerrainEditor_SetGridHeight );

    Point2I cPos;
    mTerrain->gridToCenter(gPoint, cPos);

    mGridUpdateMin.setMin( cPos );
    mGridUpdateMax.setMax( cPos );

    mTerrain->setHeight(cPos, height);
}

void TerrainDeformContext::setGridInfo(const GridInfo & info, bool checkActive)
{
    PROFILE_SCOPE( TerrainEditor_SetGridInfo );

    setGridHeight(info.mGridPoint.gridPos, info.mHeight);
    setGridMaterial(info.mGridPoint.gridPos, info.mMaterial);
}

void TerrainDeformContext::resetSelection()
{
    delete mSelection;
    mSelection = new Selection();
}

void TerrainDeformContext::scheduleMaterialUpdate()
{
    mNeedsMaterialUpdate = true;
}

void TerrainDeformContext::scheduleGridUpdate()
{
    mNeedsGridUpdate = true;
}