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

struct GridPointSet
{
    TerrainBlock* mTerrain;
    Vector<Point2I> mGridPoints;
};

class DeformationBrush
{
public:
    Point3F mPosition;

    virtual void packData(BitStream* stream);
    virtual void getGridPoints(bool isServer, Vector<GridPointSet>& out) = 0;

    F32 getAverageTerrainHeight(bool isServer);
    F32 getMinimumTerrainHeight(bool isServer);
    F32 getMaximumTerrainHeight(bool isServer);

    static DeformationBrush* unpackData(BitStream* stream);
};

class RectangleDeformationBrush : public DeformationBrush
{
    typedef DeformationBrush Parent;

public:
    //! The width of the brush.
    F32 mWidth;

    //! The height of the brush.
    F32 mHeight;

    RectangleDeformationBrush();
    RectangleDeformationBrush(const RectangleDeformationBrush& copied);
    RectangleDeformationBrush(F32 width, F32 height);

    void packData(BitStream* stream) override;
    static RectangleDeformationBrush* unpackData(BitStream* stream);

    void getGridPoints(bool isServer, Vector<GridPointSet>& out) override;
};

class DeformationAction
{
public:
    //! The brush utilized by this action.
    DeformationBrush* mBrush;

    DeformationAction(DeformationBrush* brush);

    virtual void packData(BitStream* stream) = 0;
    virtual void applyAction(bool isServer) = 0;

    static DeformationAction* unpackData(BitStream* stream);
};

class FlattenAction : public DeformationAction
{
    typedef DeformationAction Parent;

public:
    FlattenAction(DeformationBrush* brush);

    void packData(BitStream* stream) override;
    void applyAction(bool isServer) override;

    static FlattenAction* unpackData(BitStream* stream);
};

class AdjustHeightAction : public DeformationAction
{
    typedef DeformationAction Parent;

public:
    F32 mHeight;

    AdjustHeightAction(F32 height, DeformationBrush* brush);

    void packData(BitStream* stream) override;
    void applyAction(bool isServer) override;

    static AdjustHeightAction* unpackData(BitStream* stream);
};

enum DeformationActionIDs
{
    FlattenActionID,
    AdjustHeightActionID,
    PaintNoiseActionID
};

enum DeformationBrushIDs
{
    RectangleBrushID,
    CircleBrushID
};

class DeformationManager : public NetObject
{
    typedef NetObject Parent;

protected:

    Map<NetConnection*, U32> mNetConnectionState;

    // SimObject
    DECLARE_CONOBJECT(DeformationManager);

    static void initPersistFields();

    //! Actions to be networked to all connected clients.
    VectorPtr<DeformationAction*> mActions;

    // NetObject
    U32  packUpdate(NetConnection *conn, U32 mask, BitStream* stream);
    void unpackUpdate(NetConnection *conn, BitStream* stream);

public:
    DeformationManager();

    static DeformationManager* getDeformationManager();
    void registerAction(DeformationAction* action);

};
