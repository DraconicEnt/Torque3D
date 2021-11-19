#pragma once

#ifndef _TORQUE_STRING_H_
#include "core/util/str.h"
#endif
#ifndef _TSIGNAL_H_
#include "core/util/tSignal.h"
#endif
#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif

class SensorManager : public ITickable
{
public:

   SensorManager( );

   virtual ~SensorManager();

   /// This method is called every frame and lets the control interpolate between
   /// ticks so you can smooth things as long as isProcessingTicks returns true
   /// when it is called on the object
   void interpolateTick(F32 delta);

   /// This method is called once every 32ms if isProcessingTicks returns true
   /// when called on the object
   void processTick();

   /// This method is called once every frame regardless of the return value of
   /// isProcessingTicks and informs the object of the passage of time.
   /// @param timeDelta Time increment in seconds.
   void advanceTime(F32 timeDelta);

   /// The current active light manager.
   static SensorManager* smActiveSensorManager;
};
