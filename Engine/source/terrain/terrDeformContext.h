#pragma once

#ifndef _GUIFILTERCTRL_H_
#include "gui/editor/guiFilterCtrl.h"
#endif
#ifndef _UNDO_H_
#include "util/undo.h"
#endif
#ifndef _NOISE2D_H_
#include "util/noise2d.h"
#endif

#include "terrain/terrSelection.h"

class TerrainEditor;

struct TerrainDeformContext
{
    bool mNeedsGridUpdate;
    bool mNeedsMaterialUpdate;

    //! Target terrain block when processing manipulations.
    TerrainBlock* mTerrain;
    Selection* mUndoSelection;
    Selection* mSelection;
    Point2I  mGridUpdateMin;
    Point2I  mGridUpdateMax;

    //! Optional editor instance associated with this context. Required for certain actions to work.
    TerrainEditor* mEditor;

    TerrainDeformContext(TerrainEditor* editor = NULL);

    ~TerrainDeformContext();

    void setGridMaterial( const Point2I &gPoint, U8 index );

    void setGridHeight(const Point2I & gPoint, const F32 height);

    void setGridInfo(const GridInfo & info, bool checkActive);

    // Returns a list of infos for all points on the terrain that are at that point in space
    void getGridInfos(const GridPoint & gPoint, Vector<GridInfo>& infos);

    inline void setGridInfoHeight(const GridInfo & info)
    {
        setGridHeight(info.mGridPoint.gridPos, info.mHeight);
    }

    void resetSelection();

    void scheduleGridUpdate();
    void scheduleMaterialUpdate();
};
