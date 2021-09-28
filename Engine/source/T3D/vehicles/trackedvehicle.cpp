//-----------------------------------------------------------------------------
// Copyright (c) 2021 Draconic Entity
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "T3D/vehicles/trackedVehicle.h"

#include "math/mMath.h"
#include "math/mathIO.h"
#include "console/simBase.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "collision/clippedPolyList.h"
#include "collision/planeExtractor.h"
#include "core/stream/bitStream.h"
#include "core/dnet.h"
#include "T3D/gameBase/gameConnection.h"
#include "ts/tsShapeInstance.h"
#include "T3D/fx/particleEmitter.h"
#include "sfx/sfxSystem.h"
#include "sfx/sfxTrack.h"
#include "sfx/sfxSource.h"
#include "sfx/sfxTypes.h"
#include "scene/sceneManager.h"
#include "core/resourceManager.h"
#include "materials/materialDefinition.h"
#include "materials/baseMatInstance.h"
#include "lighting/lightQuery.h"


//----------------------------------------------------------------------------
// Tracked Vehicle Data Block
//----------------------------------------------------------------------------

IMPLEMENT_CO_DATABLOCK_V1(TrackedVehicleData);

ConsoleDocClass(TrackedVehicleData,
   "@brief Defines the properties of a TrackedVehicle.\n\n"
   "@ingroup Vehicles\n"
);

//----------------------------------------------------------------------------

void TrackedVehicleData::initPersistFields()
{

   Parent::initPersistFields();
}


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Tracked Vehicle Class
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

IMPLEMENT_CO_NETOBJECT_V1(TrackedVehicle);

ConsoleDocClass(TrackedVehicle,
   "@brief A tracked vehicle.\n"
   "@ingroup Vehicles\n"
);

bool TrackedVehicle::onNewDataBlock(GameBaseData* dptr, bool reload)
{
   const bool result = Parent::onNewDataBlock(dptr, reload);

   if (!result)
   {
      return false;
   }

   mDataBlock = dynamic_cast<TrackedVehicleData*>(dptr);
   return bool(mDataBlock);
}

bool TrackedVehicle::shouldPowerWheel(Wheel* wheel, bool rightSide)
{
   if (mSteering.x == 0.0f)
   {
      return true;
   }

   // If steering is negative, right tires must activate and vice versa
   if (mSteering.x < 0.0f && rightSide)
   {
      return true;
   }

   if (mSteering.x > 0.0f && !rightSide)
   {
      return true;
   }


   return false;
}

void TrackedVehicle::initPersistFields()
{
   Parent::initPersistFields();
}
