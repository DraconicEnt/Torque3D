#pragma once

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif
#ifndef _SIMDATABLOCK_H_
#include "console/simDatablock.h"
#endif
#ifndef _BITSTREAM_H_
#include "core/stream/bitStream.h"
#endif
#ifndef _GAMEBASE_H_
#include "T3D/gameBase/gameBase.h"
#endif
#ifndef _TERRDATA_H_
#include "terrain/terrData.h"
#endif

class TerrainDeformerData : public GameBaseData
{
   typedef GameBaseData Parent;

public:
   S32 type;

   enum TerrainDeformerConsts
   {
      Point,
      Rectangle
   };

   S32 lifetime;

public:
   TerrainDeformerData();
   ~TerrainDeformerData();

   DECLARE_CONOBJECT(TerrainDeformerData);
   static void initPersistFields();

   // SimDataBlock.
   virtual bool onAdd();
   virtual void packData(BitStream* stream);
   virtual void unpackData(BitStream* stream);
};

class TerrainDeformer : public GameBase
{
   typedef GameBase Parent;

private:
   TerrainDeformerData* mDataBlock;

protected:
   bool onAdd();
   void onRemove();

   void processTick(const Move* move);
   void advanceTime(F32 dt);

   Point3F mPosition;
   Point3F mAxis;

   SimObjectPtr<TerrainBlock> mTerrainBlock;
   S32 mTerrainID;

   //TerrainBlock* mTerrainBlock;
   //TerrainBlock* getClientTerrain();

   Point2I  mGridUpdateMin;
   Point2I  mGridUpdateMax;

   F32 mDepth;
   F32 mDeltaDepth;
   F32 mWidth;
   F32 mHeight;

   bool mTerrainDeformed;

   // conversion functions
   bool gridToWorld(const Point2I& gPos, Point3F& wPos);
   bool worldToGrid(const Point3F& wPos, Point2I& gPos);
   bool gridToCenter(const Point2I& gPos, Point2I& cPos);

   // height functions
   F32 getGridHeight(const Point2I& gPos);
   void setGridHeight(const Point2I& gPos, const F32 height);
   void gridUpdateComplete();

   // all deformations done in these functions
   void DeformPoint();
   void DeformRectangle();

   U32  packUpdate(NetConnection* conn, U32 mask, BitStream* stream);
   void unpackUpdate(NetConnection* conn, BitStream* stream);

public:
   TerrainDeformer();
   ~TerrainDeformer();
   bool onNewDataBlock(GameBaseData* dptr, bool reload);

   DECLARE_CONOBJECT(TerrainDeformer);
   static void initPersistFields();
};
