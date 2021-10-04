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

TrackedVehicleData::TrackedVehicleData() : WheeledVehicleData()
{
    maxTurnForce = 29000;
}

void TrackedVehicleData::initPersistFields()
{
    addField( "maxTurnForce", TYPEID< F32 >(), Offset(maxTurnForce, TrackedVehicleData),
              "Maximum force to be used when spinning the vehicle in place." );

   Parent::initPersistFields();
}

void TrackedVehicleData::packData(BitStream* stream)
{
    Parent::packData(stream);

    stream->write(maxTurnForce);
}

void TrackedVehicleData::unpackData(BitStream* stream)
{
    Parent::unpackData(stream);

    stream->read(&maxTurnForce);
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

void TrackedVehicle::updateForces(F32 dt)
{
    Parent::updateForces(dt);

    // Enumerate wheels and accumulate turning force
    float turningForce = 0.0f;
    const float turningForcePerWheel = mDataBlock->maxTurnForce / mDataBlock->wheelCount;

    Wheel* lastWheel = &mWheel[mDataBlock->wheelCount];
    for (Wheel* wheel = mWheel; wheel < lastWheel; wheel++)
    {
        if (!wheel->tire || !wheel->spring)
            continue;

        if (wheel->surface.contact)
        {
            turningForce += turningForcePerWheel;
        }
    }

    mRigid.torque += Point3F(0.0f, 0.0f, turningForce * -mSteering.x);
    mRigid.angMomentum += mRigid.torque * dt;
    mRigid.updateVelocity();
}

void TrackedVehicle::initPersistFields()
{
   Parent::initPersistFields();
}
