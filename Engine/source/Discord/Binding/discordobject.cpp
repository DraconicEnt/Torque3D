#include "console/engineAPI.h"

#include "discordobject.h"

IMPLEMENT_CONOBJECT(DiscordObject);

ConsoleDocClass(DiscordObject,
   "@brief Object used to interact with the discord API.\n\n"
);

DiscordObject::DiscordObject() : mDiscordCore(nullptr), mCurrentStatusState(nullptr), mCurrentStatusDetails(nullptr)
{
   auto testResult = discord::Core::Create(TORQUE_DISCORD_APPLICATION_ID, DiscordCreateFlags_Default, &mDiscordCore);

   // Create discord core
   discord::Activity activity{};
   mDiscordCore->ActivityManager().UpdateActivity(activity, [this](discord::Result result) {
      onStatusUpdate_callback();
   });
}

void DiscordObject::interpolateTick(F32 delta)
{

}

void DiscordObject::processTick()
{
   mDiscordCore->RunCallbacks();
}

void DiscordObject::advanceTime(F32 timeDelta)
{

}

void DiscordObject::setStatusState(const char* state)
{
   if (mCurrentStatusState)
   {
      delete mCurrentStatusState;
   }

   mCurrentStatusState = new char[dStrlen(state)];
   dMemcpy(mCurrentStatusState, state, dStrlen(state));
}

void DiscordObject::setStatusDetails(const char* details)
{
   if (mCurrentStatusDetails)
   {
      delete mCurrentStatusDetails;
   }

   mCurrentStatusDetails = new char[dStrlen(details)];
   dMemcpy(mCurrentStatusDetails, details, dStrlen(details));
}

void DiscordObject::updateStatus()
{
   discord::Activity activity{};
   if (mCurrentStatusDetails)
   {
      activity.SetDetails(mCurrentStatusDetails);
   }
   if (mCurrentStatusState)
   {
      activity.SetState(mCurrentStatusState);
   }

   mDiscordCore->ActivityManager().UpdateActivity(activity, [this](discord::Result result) {
      onStatusUpdate_callback();
   });
}

// =====================================================================================================================
IMPLEMENT_CALLBACK(DiscordObject, onStatusUpdate, void, (), (),
   "Called when the discord status is updated.\n"
);

// =====================================================================================================================
DefineEngineMethod(DiscordObject, setStatusState, void, (const char* state), ,
   "@brief Sets a new activity status state.\n\n")
{
   object->setStatusState(state);
}

DefineEngineMethod(DiscordObject, setStatusDetails, void, (const char* details), ,
   "@brief Sets a new activity status details.\n\n")
{
   object->setStatusDetails(details);
}

DefineEngineMethod(DiscordObject, updateStatus, bool, (), ,
   "@brief Triggers the actual discord status update.\n\n")
{
   object->updateStatus();
   return true;
}

