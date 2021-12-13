#pragma once

#ifndef _GAMEBASE_H_
#include "T3D/gameBase/gameBase.h"
#endif

#ifndef _TERRDATA_H_
#include "terrain/terrData.h"
#endif

#ifndef _MRANDOM_H_
#include "math/mRandom.h"
#endif

#include "BOL/game/deformationManager.h"

class TerrainBrush : public GameBase
{
    typedef GameBase Parent;

protected:
    struct GridPointSet
    {
        TerrainBlock* mTerrain;
        Vector<Point2I> mGridPoints;
    };

    // SimObject
    DECLARE_CONOBJECT(TerrainBrush);

    static void initPersistFields();

    // NetObject
    U32  packUpdate(NetConnection *conn, U32 mask, BitStream* stream);
    void unpackUpdate(NetConnection *conn, BitStream* stream);

    // Action code
    virtual void adjustHeight(F32 height);
    virtual void flatten();
    virtual void paintNoise(F32 heightVariation, S32 seed = 0);
};

class RectangleTerrainBrush : public TerrainBrush
{
    typedef TerrainBrush Parent;

protected:
    //! The brush associated with this script accessible brush object.
    RectangleDeformationBrush mBrush;

    // SimObject
    DECLARE_CONOBJECT(RectangleTerrainBrush);

    // NetObject
    U32  packUpdate(NetConnection *conn, U32 mask, BitStream* stream);
    void unpackUpdate(NetConnection *conn, BitStream* stream);

    static void initPersistFields();

    // Action code
    virtual void adjustHeight(F32 height);
    virtual void flatten();
    virtual void paintNoise(F32 heightVariation, S32 seed = 0);
};
