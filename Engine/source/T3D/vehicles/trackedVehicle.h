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

   TrackedVehicleData();
   DECLARE_CONOBJECT(TrackedVehicleData);

   F32 maxTurnForce;

   void packData(BitStream* stream) override;
   void unpackData(BitStream* stream) override;


   static void initPersistFields();
};


//----------------------------------------------------------------------------

class TrackedVehicle : public WheeledVehicle
{
   typedef WheeledVehicle Parent;

   TrackedVehicleData* mDataBlock;

   bool onNewDataBlock(GameBaseData* dptr, bool reload);

   void updateForces(F32 dt) override;
public: 
   DECLARE_CONOBJECT(TrackedVehicle);
   static void initPersistFields();
};


#endif
