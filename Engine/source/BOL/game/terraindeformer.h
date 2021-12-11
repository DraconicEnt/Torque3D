//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _TERRAINDEFORMER_H_
#define _TERRAINDEFORMER_H_

#ifndef _GAMEBASE_H_
#include "T3D/gameBase/gameBase.h"
#endif

#include "terrain/terrData.h"
//#include "game/fx/particleEmitter.h"

//#ifndef _PLATFORMAUDIO_H_
//#include "platform/platformAudio.h"
//#endif

class ParticleEmitter;
class ParticleEmitterData;
class AudioProfile;

//--------------------------------------------------------------------------
class TerrainDeformerData : public GameBaseData {
  public:
   typedef GameBaseData Parent;

   S32 type;

   enum TerrainDeformerConsts
   {
      Point,
	  Rectangle,
   };

   S32 lifetime;

  public:
   
   TerrainDeformerData();
   DECLARE_CONOBJECT(TerrainDeformerData);
   bool onAdd();
   static void  initPersistFields();
   virtual void packData(BitStream* stream);
   virtual void unpackData(BitStream* stream);
};


//--------------------------------------------------------------------------
class TerrainDeformer : public GameBase
{
   typedef GameBase Parent;

  public:
   // Initial conditions
   enum TerrainDeformerConstants {
      MaxSoundTicks       = 4095,
	  MaxLivingTicks       = 4095,
   };

  private:
   TerrainDeformerData*   mDataBlock;
         
  protected:
   bool onAdd();
   void onRemove();
   
   void processTick(const Move *move);
   void advanceTime(F32 dt);
      
   Point3F mPosition;
   Point3F mAxis;

   S32 mTerrainID;

  // AUDIOHANDLE mSoundHandle;
   U32 mCurrTick;                         ///< Current time in ticks

   U32  packUpdate  (NetConnection *conn, U32 mask, BitStream *stream);
   void unpackUpdate(NetConnection *conn,           BitStream *stream);
   
   TerrainBlock* mTerrainBlock;
   TerrainBlock * getClientTerrain();

   Point2I  mGridUpdateMin;
   Point2I  mGridUpdateMax;

   F32 mDepth;
   F32 mDeltaDepth;
   F32 mWidth;
   F32 mHeight;

   
   bool mTerrainDeformed;

   // conversion functions
   bool gridToWorld(const Point2I & gPos, Point3F & wPos);
   bool worldToGrid(const Point3F & wPos, Point2I & gPos);
   bool gridToCenter(const Point2I & gPos, Point2I & cPos);

   // height functions
   F32 getGridHeight(const Point2I & gPos);
   void setGridHeight(const Point2I & gPos, const F32 height);
   void gridUpdateComplete();

   // all deformations done in these functions
   void DeformPoint();
   void DeformRectangle();
                  
  public:
   TerrainDeformer();
   ~TerrainDeformer();
   bool onNewDataBlock(GameBaseData* dptr, bool reload);
   void UnDeform();//Nelson Undo Deformations

   DECLARE_CONOBJECT(TerrainDeformer);
   static void initPersistFields();
};

#endif // _TERRAINDEFORMER_H_

