//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "BOL/game/terraindeformer.h"

#include "console/consoleTypes.h"
#include "console/typeValidators.h"
// #include "dgl/dgl.h"
//#include "platform/platformAudio.h"
//#include "audio/audioDataBlock.h"
//#include "sceneGraph/sceneGraph.h"
//#include "sceneGraph/sceneState.h"
#include "core/stream/bitStream.h"
#include "sim/netConnection.h"
#include "ts/tsShape.h"
#include "ts/tsShapeInstance.h"
#include "math/mRandom.h"
#include "math/mathIO.h"
#include "math/mathUtils.h"
#include "T3D/debris.h"
#include "T3D/gamebase/gameConnection.h"
//#include "T3D/fx/particleEngine.h"
//#include "game/fx/cameraFXMgr.h"
#include "T3D/decal/decalManager.h"

IMPLEMENT_CO_DATABLOCK_V1(TerrainDeformerData);
IMPLEMENT_CO_NETOBJECT_V1(TerrainDeformer);

//----------------------------------------------------------------------------
//

TerrainDeformerData::TerrainDeformerData()
{
   type = TerrainDeformerData::Point;

   lifetime = 20000 / 32;
}

void TerrainDeformerData::initPersistFields()
{
   Parent::initPersistFields();

   addNamedField(type, TypeS32, TerrainDeformerData);

   //addNamedField(particleEmitter,  TypeParticleEmitterDataPtr, TerrainDeformerData);

 //  addNamedField(sound, TypeAudioProfilePtr, TerrainDeformerData);
   //addNamedFieldV(soundLifetime, TypeS32, TerrainDeformerData, new IRangeValidatorScaled(TickMs, 0, TerrainDeformer::MaxSoundTicks));

   addNamedFieldV(lifetime, TypeS32, TerrainDeformerData, new IRangeValidatorScaled(TickMs, 1, TerrainDeformer::MaxLivingTicks));
}

bool TerrainDeformerData::onAdd()
{
   if(!Parent::onAdd())
      return false;

   return true;
}

void TerrainDeformerData::packData(BitStream* stream)
{
   Parent::packData(stream);

   stream->writeInt(type, 2);

  // if (stream->writeFlag(particleEmitter != NULL))
  //    stream->writeRangedU32(particleEmitter->getId(), DataBlockObjectIdFirst, DataBlockObjectIdLast);

  // if (stream->writeFlag(sound != NULL))
  //    stream->writeRangedU32(sound->getId(), DataBlockObjectIdFirst, DataBlockObjectIdLast);
   //stream->writeRangedU32(soundLifetime, 0, TerrainDeformer::MaxSoundTicks);

   stream->writeRangedU32(lifetime, 0, TerrainDeformer::MaxLivingTicks);
}

void TerrainDeformerData::unpackData(BitStream* stream)
{
   Parent::unpackData(stream);

   type = stream->readInt(2);

 //  if (stream->readFlag())
  //    particleEmitterId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);

//  if (stream->readFlag())
 //     soundId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);
 //  soundLifetime = stream->readRangedU32(0, TerrainDeformer::MaxSoundTicks);

   lifetime = stream->readRangedU32(0, TerrainDeformer::MaxLivingTicks);
}

//--------------------------------------------------------------------------

TerrainDeformer::TerrainDeformer()
{
   mNetFlags.set(Ghostable | ScopeAlways);
   mTypeMask |= TerrainDeformerObjectType;

   mTerrainBlock = NULL;
   mGridUpdateMin = Point2I(255, 255);
   mGridUpdateMax = Point2I(0, 0);

   mWidth = 0.0;
   mHeight = 0.0;
   mDepth = 0.0;
   
   mDeltaDepth = 0.0;
   mTerrainDeformed = false;

   mPosition = Point3F(0, 0, 0);
   mAxis = Point3F(0, 0, 0);

   mTerrainID = -1;

  // mSoundHandle = NULL_AUDIOHANDLE;
   mCurrTick = 0;
}

TerrainDeformer::~TerrainDeformer()
{
   
}

//--------------------------------------------------------------------------
void TerrainDeformer::initPersistFields()
{
   Parent::initPersistFields();
   addGroup("Physics");
      addField("width",  TypeF32, Offset(mWidth, TerrainDeformer));
      addField("height",  TypeF32, Offset(mHeight, TerrainDeformer));
      addField("depth",  TypeF32, Offset(mDepth, TerrainDeformer));
   endGroup("Physics");

   addField("terrain", TypeS32, Offset(mTerrainID, TerrainDeformer));
   //
}

//--------------------------------------------------------------------------
bool TerrainDeformer::onAdd()
{
   if (!Parent::onAdd())
      return false;

   mCurrTick = 0;

   mDeltaDepth = mDepth / (F32)mDataBlock->lifetime;

   if (isServerObject())
   {
      MatrixF mat = getTransform();
      mat.getColumn(1, &mAxis);
      mat.getColumn(3, &mPosition);

      setMaskBits(InitialUpdateMask);
   }
   else
   {
      /*
      if (mDataBlock->particleEmitter != NULL)
      {
         ParticleEmitter* pEmitter = new ParticleEmitter;
         pEmitter->onNewDataBlock(mDataBlock->particleEmitter);
         if (pEmitter->registerObject() == false)
         {
            Con::warnf(ConsoleLogEntry::General, "Could not register particle emitter for particle of class: %s", mDataBlock->getName());
            delete pEmitter;
            pEmitter = NULL;
         }
         mParticleEmitter = pEmitter;
      }
      */
   }

   // get the terrain block
   // FIXME: Get the terrain block
  // mTerrainBlock = c;
   //if (mTerrainBlock == NULL)
   //   Con::warnf(ConsoleLogEntry::General, "Could not get terrain for class: %s", mDataBlock->getName());

   if (isServerObject())
   {
      if (!Sim::findObject<TerrainBlock>(mTerrainID, mTerrainBlock))
      {
         Con::errorf("Invalid terrain ID!");
      }
   }
   
   mObjBox = Box3F(Point3F(0, 0, 0), Point3F(0, 0, 0));
   resetWorldBox();

   addToScene();

   return true;
}

void TerrainDeformer::onRemove()
{
   /*
   if (bool(mParticleEmitter)) {
      mParticleEmitter->deleteWhenEmpty();
      mParticleEmitter = NULL;
   }

   if (mSoundHandle != NULL_AUDIOHANDLE) {
      alxStop(mSoundHandle);
      mSoundHandle = NULL_AUDIOHANDLE;
   }
   */

   removeFromScene();
   Parent::onRemove();
}


bool TerrainDeformer::onNewDataBlock(GameBaseData* dptr, bool reload)
{
   mDataBlock = dynamic_cast<TerrainDeformerData*>(dptr);
   if (!mDataBlock || !Parent::onNewDataBlock(dptr, reload))
      return false;

   return true;
}


//--------------------------------------------------------------------------
void TerrainDeformer::processTick(const Move* move)
{
   Parent::processTick(move);

   mCurrTick++;

   if (!mTerrainBlock)
      return;

   if (!mTerrainDeformed)
   {
      // we're only going to do this once. this means that once a deformer is placed,
      // then moving it will have no impact.
      
      // get the position of the deformer
      MatrixF mat = getTransform();
      Point3F pos;
      mat.getColumn(3, &pos);

      // get the grid position from the world position
      Point2I gPos;
      if (!worldToGrid(pos, gPos))
	      return;
    
      // get a new world position centered over the deformation.
      Point3F wPos;
      gridToWorld(gPos, wPos);
    
      // move the position of this object to the new world position
      // so that emitters are positioned over the center of the crater
      mat.setColumn(3, wPos);
      setTransform(mat);
    
      if (mCurrTick <= mDataBlock->lifetime)
      {
         // perform the desired type of deformation
         switch (mDataBlock->type)
         {
         case TerrainDeformerData::Rectangle:
            DeformRectangle();
            break;
    
         default:
	        DeformPoint();
	        break;
         }
         // NOTE: Objects will not be notified until AFTER the terrain has been deformed.
         // if we're processing over time, then you may want to inform objects a few times
         // during the deform perhaps via a mInformObjects flag that's set here every so often
         // although probably best not to update objects EVERY tick of deforming ;) Once a second
         // or so should do

      }
      else      
         mTerrainDeformed = true; 
       
   }
   else
   {
      // The terrain has been deformed. As most objects will cache their working collision set
      // and only change it upon moving a certain amount we'll notify these objects of the 
      // terrain change and leave it up to them to invalidate their working sets. 
      SceneContainer *cont = isServerObject() ? &gServerContainer : &gClientContainer; 
      Box3F queryBox(mPosition, mPosition);
      queryBox.minExtents -= Point3F(0, 0, 0);
      queryBox.maxExtents += Point3F(mWidth, mWidth, mWidth);

      SimpleQueryList queryList;
      cont->findObjects(queryBox, ShapeBaseObjectType, SimpleQueryList::insertionCallback, &queryList);

     // for (U32 i = 0; i < queryList.mList.size(); i++) 
     // { 
     //    ShapeBase* sel = static_cast<ShapeBase*>(queryList.mList[i]);

     //    sel->terrainDeformed();
    //  }

	  //Clearing decals in radius
	  if(gDecalManager)
	  {
		//  F32 adjustedWidth = mWidth * 1.25;
    //    gDecalManager->checkForDecalTD(mPosition, adjustedWidth);
	  }

      // We've fully deformed terrain AND now notified all affected objects, nothing more to do
      setProcessTick(false);
   }
}

void TerrainDeformer::advanceTime(F32 dt)
{
   Parent::advanceTime(dt);

   if (dt == 0.0)
      return;

}

//--------------------------------------------------------------------------
U32 TerrainDeformer::packUpdate(NetConnection* con, U32 mask, BitStream* stream)
{
   U32 retMask = Parent::packUpdate(con, mask, stream);

   if (stream->writeFlag(mask & GameBase::InitialUpdateMask))
   {
      stream->writeCompressedPoint(mPosition);
	   stream->writeCompressedPoint(mAxis);
      stream->writeFloat(mWidth / 10, 10);
	   stream->writeFloat(mHeight / 10, 10);
	   stream->writeFloat(mDepth / 10, 10);

      S32 ghostIndex = con->getGhostIndex(mTerrainBlock);
      if (stream->writeFlag(ghostIndex != -1))
      {
         stream->writeRangedU32(U32(ghostIndex), 0, NetConnection::MaxGhostCount);
      }
   }

   return retMask;
}

void TerrainDeformer::unpackUpdate(NetConnection* con, BitStream* stream)
{
   Parent::unpackUpdate(con, stream);

   // initial update
   if (stream->readFlag())
   {
      stream->readCompressedPoint(&mPosition);
	   stream->readCompressedPoint(&mAxis);
	   setPosition(mPosition);
      mWidth = stream->readFloat(10);
      mWidth *= 10;
	   mHeight = stream->readFloat(10);
      mHeight *= 10;
	   mDepth = stream->readFloat(10);
	   mDepth *= 10;

      if (stream->readFlag())
      {
         mTerrainID = stream->readRangedU32(0, NetConnection::MaxGhostCount);
         NetObject* receivedObject = con->resolveGhost(mTerrainID);
         mTerrainBlock = dynamic_cast<TerrainBlock*>(receivedObject);
      }
   }
}

//----------------------------------------------------------------------------
// functions ripped from terrainEditor

TerrainBlock * TerrainDeformer::getClientTerrain()
{
   // do the client..

   NetConnection * toServer = NetConnection::getConnectionToServer();
   NetConnection * toClient = NetConnection::getLocalClientConnection();

   S32 index = toClient->getGhostIndex(mTerrainBlock);

   return(dynamic_cast<TerrainBlock*>(toServer->resolveGhost(index)));
}

bool TerrainDeformer::gridToWorld(const Point2I & gPos, Point3F & wPos)
{
   const MatrixF & mat = mTerrainBlock->getTransform();
   Point3F origin;
   mat.getColumn(3, &origin);

   wPos.x = gPos.x * (float)mTerrainBlock->getSquareSize() + origin.x;
   wPos.y = gPos.y * (float)mTerrainBlock->getSquareSize() + origin.y;
   wPos.z = getGridHeight(gPos) + origin.z;
      
   return true;
}

bool TerrainDeformer::worldToGrid(const Point3F & wPos, Point2I & gPos)
{
   gPos = mTerrainBlock->getGridPos(wPos);

   /*
   const MatrixF & mat = mTerrainBlock->getTransform();
   Point3F origin;
   mat.getColumn(3, &origin);
   F32 squareSize = (F32) mTerrainBlock->getSquareSize();
   F32 halfSquareSize = squareSize / 2;

   float x = (wPos.x - origin.x + halfSquareSize) / squareSize;
   float y = (wPos.y - origin.y + halfSquareSize) / squareSize;
   
   gPos.x = (S32)mFloor(x);
   gPos.y = (S32)mFloor(y);

   mTerrainBlock->getGridPos(   
   return(!(gPos.x >> TerrainBlock::BlockShift || gPos.y >> TerrainBlock::BlockShift));
   */

   // FIXME: Load worldToGrid from terrainEditor
   return true;
}

bool TerrainDeformer::gridToCenter(const Point2I & gPos, Point2I & cPos)
{
   cPos.x = gPos.x;
   cPos.y = gPos.y;
   
   return true;
}

F32 TerrainDeformer::getGridHeight(const Point2I & gPos)
{
   Point2I cPos;
   gridToCenter(gPos, cPos);

   const TerrainFile* file = mTerrainBlock->getFile();
   return fixedToFloat(file->getHeight(cPos.x, cPos.y));
}

void TerrainDeformer::setGridHeight(const Point2I & gPos, const F32 height)
{
   Point2I cPos;
   gridToCenter(gPos, cPos);
   if(cPos.x < mGridUpdateMin.x)
      mGridUpdateMin.x = cPos.x;
   if(cPos.y < mGridUpdateMin.y)
      mGridUpdateMin.y = cPos.y;
   if(cPos.x > mGridUpdateMax.x)
      mGridUpdateMax.x = cPos.x;
   if(cPos.y > mGridUpdateMax.y)
      mGridUpdateMax.y = cPos.y;

   mTerrainBlock->setHeight(cPos, height);
}

void TerrainDeformer::gridUpdateComplete()
{
   if(mGridUpdateMin.x <= mGridUpdateMax.x)
      mTerrainBlock->updateGrid(mGridUpdateMin, mGridUpdateMax);
   mGridUpdateMin.set(256,256);
   mGridUpdateMax.set(0,0);
}

//----------------------------------------------------------------------------

void TerrainDeformer::DeformPoint()
{
   // get the position of the deformer
   MatrixF mat = getTransform();
   Point3F pos;
   mat.getColumn(3, &pos);

   // get the grid position from the world position
   Point2I gPos;
   if (!worldToGrid(pos, gPos))
      return;

   // get the height of the terrain at the grid position
   float height = getGridHeight(gPos);
		 
   // set the height of the terrain at the grid position
   setGridHeight(gPos, height - mDeltaDepth);

   gridUpdateComplete();
}

void TerrainDeformer::DeformRectangle()
{
   // get the world position of the deformer
   MatrixF mat = getTransform();
   Point3F pos;
   mat.getColumn(3, &pos);

   // get the left, right
   // world coords
   F32 left, right;
   left = pos.x - mWidth / 2.0;
   right = pos.x + mWidth / 2.0;

   // get the top and bottom
   // world coords
   F32 top, bottom;
   top = pos.y - mHeight / 2.0;
   bottom = pos.y + mHeight / 2.0;

   // get the topleft, and bottom right
   // terrain coords
   Point2I gTopLeft;
   Point2I gBottomRight;

   if (!worldToGrid(Point3F(left, top, 0.0), gTopLeft) ||
	   !worldToGrid(Point3F(right, bottom, 0.0), gBottomRight))
      return;

   Point2I gPos;
   // iterate over the terrain points
   for (gPos.x = gTopLeft.x; gPos.x <= gBottomRight.x; gPos.x ++)
   {
      for (gPos.y = gTopLeft.y; gPos.y <= gBottomRight.y; gPos.y ++)
	  {
         // get the height of the terrain at the grid position
         float height = getGridHeight(gPos);
		 
         // set the height of the terrain at the grid position
         setGridHeight(gPos, height - mDeltaDepth);
	  }
   }

   //
   gridUpdateComplete();
}

//Nelson Undo Deformations
void TerrainDeformer::UnDeform()
{
	for (U32 i = 0; i <= mDataBlock->lifetime; i++) 
	{
		mDeltaDepth = mDepth / (F32)mDataBlock->lifetime;
		S32 unDoType = (S32)mDataBlock->type;
		
		
		// get the position of the deformer
		MatrixF mat = getTransform();
		Point3F pos;
		mat.getColumn(3, &pos);
		
		if(unDoType == 0)
		{
			// get the grid position from the world position
			Point2I gPos;
			if (!worldToGrid(pos, gPos))
				return;
			
			// get the height of the terrain at the grid position
			float height = getGridHeight(gPos);
			
			// set the height of the terrain at the grid position
			setGridHeight(gPos, height + mDeltaDepth);
			
			gridUpdateComplete();
		}
		else if(unDoType == 1)
		{
			// get the left, right
			// world coords
			F32 left, right;
			left = pos.x - mWidth / 2.0;
			right = pos.x + mWidth / 2.0;
			
			// get the top and bottom
			// world coords
			F32 top, bottom;
			top = pos.y - mHeight / 2.0;
			bottom = pos.y + mHeight / 2.0;
			
			// get the topleft, and bottom right
			// terrain coords
			Point2I gTopLeft;
			Point2I gBottomRight;
			
			if (!worldToGrid(Point3F(left, top, 0.0), gTopLeft) ||
				!worldToGrid(Point3F(right, bottom, 0.0), gBottomRight))
				return;
			
			Point2I gPos;
			// iterate over the terrain points
			for (gPos.x = gTopLeft.x; gPos.x <= gBottomRight.x; gPos.x ++)
			{
				for (gPos.y = gTopLeft.y; gPos.y <= gBottomRight.y; gPos.y ++)
				{
					// get the height of the terrain at the grid position
					float height = getGridHeight(gPos);
				
					// set the height of the terrain at the grid position
					setGridHeight(gPos, height + mDeltaDepth);
				}
			}
			
			gridUpdateComplete();
		}
	}

	// The terrain has been deformed. As most objects will cache their working collision set
	// and only change it upon moving a certain amount we'll notify these objects of the 
	// terrain change and leave it up to them to invalidate their working sets. 
	SceneContainer *cont = isServerObject() ? &gServerContainer : &gClientContainer; 
	Box3F queryBox(mPosition, mPosition);
	queryBox.minExtents -= Point3F(0, 0, 0);
	queryBox.maxExtents += Point3F(mWidth, mWidth, mWidth);

	SimpleQueryList queryList;
	cont->findObjects(queryBox, ShapeBaseObjectType, SimpleQueryList::insertionCallback, &queryList);

   /*
	for (U32 i = 0; i < queryList.mList.size(); i++) 
	{ 
		ShapeBase* sel = static_cast<ShapeBase*>(queryList.mList[i]);
		
		sel->terrainDeformed();
	}
   */
}

DefineEngineMethod(TerrainDeformer, UnDeform, void, (void),,
   "Undo the operation performed by this deformer.")
{
	object->UnDeform();
}
//Nelson Undo Deformations
