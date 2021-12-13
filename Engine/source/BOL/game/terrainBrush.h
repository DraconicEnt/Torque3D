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

    void getCandidateTerrains(Vector<TerrainBlock*>& out);

    // NetObject
    U32  packUpdate(NetConnection *conn, U32 mask, BitStream* stream);
    void unpackUpdate(NetConnection *conn, BitStream* stream);

    // Brush code
    virtual void getGridPoints(Vector<GridPointSet>& out);
    F32 getAverageTerrainHeight();
    F32 getMinimumTerrainHeight();
    F32 getMaximumTerrainHeight();

    void lower(F32 height);
    void flatten();
    void paintNoise(F32 heightVariation, S32 seed = 0);
};

class RectangleTerrainBrush : public TerrainBrush
{
    typedef TerrainBrush Parent;

protected:

    // Rectangle parameters
    F32 mWidth;
    F32 mHeight;

    // SimObject
    DECLARE_CONOBJECT(RectangleTerrainBrush);

    // NetObject
    U32  packUpdate(NetConnection *conn, U32 mask, BitStream* stream);
    void unpackUpdate(NetConnection *conn, BitStream* stream);

    // Brush Code
    void getGridPoints(Vector<GridPointSet>& out);

    static void initPersistFields();
};