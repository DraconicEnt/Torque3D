#include "BOL/game/terrainDeformer.h"

//----------------------------------------------------------------------------
//

IMPLEMENT_CO_DATABLOCK_V1(TerrainDeformerData);

ConsoleDocClass(TerrainDeformerData,
   "@brief A description for sensor information on a given object.\n\n"

   "@ingroup Sensor\n"
   "@ingroup Datablocks\n"
);


TerrainDeformerData::TerrainDeformerData()
{

}

TerrainDeformerData::~TerrainDeformerData()
{

}

void TerrainDeformerData::initPersistFields()
{
   Parent::initPersistFields();

}

bool TerrainDeformerData::onAdd()
{
   if (!Parent::onAdd())
   {
      return false;
   }

   // TODO: Perform basic validation
   return true;
}

void TerrainDeformerData::packData(BitStream* stream)
{
   Parent::packData(stream);


}

void TerrainDeformerData::unpackData(BitStream* stream)
{
   Parent::unpackData(stream);

}

//--------------------------------------------------------------------------

IMPLEMENT_CO_NETOBJECT_V1(TerrainDeformer);

TerrainDeformer::TerrainDeformer()
{
   mTerrainBlock = NULL;
   mTerrainID = -1;
}

TerrainDeformer::~TerrainDeformer()
{

}

bool TerrainDeformer::onAdd()
{
   if (!Parent::onAdd())
   {
      return false;
   }

   TerrainBlock* tptr;
   if (mTerrainID != -1)
   {
      if (Sim::findObject(mTerrainID, tptr))
      {
         mTerrainBlock = tptr;
      }
      else
      {
         Con::errorf("Invalid terrain ID!");
      }
   }

   addToScene();
   return true;
}


void TerrainDeformer::onRemove()
{
   removeFromScene();
   Parent::onRemove();
}

bool TerrainDeformer::onNewDataBlock(GameBaseData* dptr, bool reload)
{
   mDataBlock = dynamic_cast<TerrainDeformerData*>(dptr);
   if (!mDataBlock || !Parent::onNewDataBlock(dptr, reload))
   {
      return false;
   }

   return true;
}

void TerrainDeformer::initPersistFields()
{
   addGroup("Physics");
      addField("width", TypeF32, Offset(mWidth, TerrainDeformer));
      addField("height", TypeF32, Offset(mHeight, TerrainDeformer));
      addField("depth", TypeF32, Offset(mDepth, TerrainDeformer));
   endGroup("Physics");

   addField("terrain", TypeS32, Offset(mTerrainID, TerrainDeformer));


   Parent::initPersistFields();
}

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
      }
   }
}

void TerrainDeformer::processTick(const Move* move)
{
   Parent::processTick(move);
}

void TerrainDeformer::advanceTime(F32 dt)
{
   Parent::advanceTime(dt);
}
