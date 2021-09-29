//-----------------------------------------------------------------------------
// Copyright (c) 2021 Draconic Entity
//-----------------------------------------------------------------------------

#ifndef _TRACKEDVEHICLE_H_
#define _TRACKEDVEHICLE_H_

#ifndef _WHEELEDVEHICLE_H_
#include "T3D/vehicles/wheeledVehicle.h"
#endif

struct TrackedVehicleData : public WheeledVehicleData 
{
   typedef WheeledVehicleData Parent;

   DECLARE_CONOBJECT(TrackedVehicleData);

   static void initPersistFields();
};


//----------------------------------------------------------------------------

class TrackedVehicle : public WheeledVehicle
{
   typedef WheeledVehicle Parent;

   TrackedVehicleData* mDataBlock;

   bool onNewDataBlock(GameBaseData* dptr, bool reload);

   bool shouldPowerWheel(Wheel* wheel, bool rightSide) override;

public: 
   DECLARE_CONOBJECT(TrackedVehicle);
   static void initPersistFields();
};


#endif
