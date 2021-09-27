#ifndef _INCLUDE_DISCORDOBJECT_H_
#define _INCLUDE_DISCORDOBJECT_H_

#ifndef _CONSOLEINTERNAL_H_
#include "console/consoleInternal.h"
#endif

#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif

#include "Discord/discord.h"

class DiscordObject : public SimObject, public ITickable
{
   typedef SimObject Parent;

protected:
   discord::Core* mDiscordCore;

   char* mCurrentStatusState;
   char* mCurrentStatusDetails;
   

public:
   DiscordObject();
   DECLARE_CONOBJECT(DiscordObject);
   DECLARE_CALLBACK(void, onStatusUpdate, ());

   void setStatusState(const char* state);
   void setStatusDetails(const char* details);

   void updateStatus();

   /// This method is called every frame and lets the control interpolate between
   /// ticks so you can smooth things as long as isProcessingTicks returns true
   /// when it is called on the object
   virtual void interpolateTick(F32 delta);

   /// This method is called once every 32ms if isProcessingTicks returns true
   /// when called on the object
   virtual void processTick();

   /// This method is called once every frame regardless of the return value of
   /// isProcessingTicks and informs the object of the passage of time.
   /// @param timeDelta Time increment in seconds.
   virtual void advanceTime(F32 timeDelta);
};


#endif
