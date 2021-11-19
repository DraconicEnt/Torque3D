#include "platform/platform.h"
#include "BOL/sensorManager.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "core/util/safeDelete.h"
#include "console/sim.h"
#include "console/simSet.h"
#include "scene/sceneManager.h"
#include "materials/materialManager.h"
#include "materials/sceneData.h"
#include "T3D/gameBase/gameConnection.h"
#include "console/engineAPI.h"

SensorManager* SensorManager::smActiveSensorManager = NULL;


SensorManager::SensorManager( )
{ 

}

SensorManager::~SensorManager()
{

}


void SensorManager::interpolateTick(F32 delta)
{

}

void SensorManager::processTick()
{
   // Enumerate server container and handle sensor updates
   gServerContainer.initTypeSearch(GameBaseObjectType);

   SceneObject* currentObject;
   while (currentObject = gServerContainer.containerSearchNextObject())
   {
      GameBase* gameBaseObject = static_cast<GameBase*>(currentObject);

      GameBaseData* datablock = gameBaseObject->getDataBlock();
      if (datablock && datablock->mSensorData)
      {
         
      }
   }
}

void SensorManager::advanceTime(F32 timeDelta)
{

}

DefineEngineFunction( resetSensorManager, void, ( ),,
   "Resets the sensor manager. Should be called between missions or on client join/disconnect.\n"
   "@ingroup Sensor\n" )
{
   delete SensorManager::smActiveSensorManager;
   SensorManager::smActiveSensorManager = new SensorManager();
}
