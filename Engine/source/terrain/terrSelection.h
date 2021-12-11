#pragma once

#ifndef _EDITTSCTRL_H_
#include "gui/worldEditor/editTSCtrl.h"
#endif
#ifndef _TERRDATA_H_
#include "terrain/terrData.h"
#endif
#ifndef _UNDO_H_
#include "util/undo.h"
#endif

// Each 2D grid position must be associated with a terrainBlock
struct GridPoint
{
    Point2I        gridPos;
    TerrainBlock*  terrainBlock;

    GridPoint() { gridPos.set(0, 0); terrainBlock = NULL; };
};

class GridInfo
{
public:

    GridPoint                  mGridPoint;
    U8                         mMaterial;
    F32                        mHeight;
    F32                        mWeight;
    F32                        mStartHeight;

    bool                       mPrimarySelect;
    bool                       mMaterialChanged;

    // hash table
    S32                        mNext;
    S32                        mPrev;
};


class Selection : public Vector<GridInfo>
{
private:

    StringTableEntry     mName;
    BitSet32             mUndoFlags;

    // hash table
    S32 lookup(const Point2I & pos);
    void insert(GridInfo info);
    U32 getHashIndex(const Point2I & pos);
    bool validate();

    Vector<S32>          mHashLists;
    U32                  mHashListSize;

public:

    Selection();
    virtual ~Selection();

    void reset();

    /// add unique grid info into the selection - test uniqueness by grid position
    bool add(const GridInfo &info);
    bool getInfo(Point2I pos, GridInfo & info);
    bool setInfo(GridInfo & info);
    bool remove(const GridInfo &info);
    void setName(StringTableEntry name);
    StringTableEntry getName(){return(mName);}
    F32 getAvgHeight();
    F32 getMinHeight();
    F32 getMaxHeight();
};
