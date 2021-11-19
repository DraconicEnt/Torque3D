#include "BOL/sensorData.h"

IMPLEMENT_CO_DATABLOCK_V1(SensorData);

ConsoleDocClass(SensorData,
   "@brief A description for sensor information on a given object.\n\n"

   "@ingroup Sensor\n"
   "@ingroup Datablocks\n"
);


SensorData::SensorData() : mEffectiveRadius(0.0f), mPenetratingTypeMasks(0)
{

}

SensorData::~SensorData()
{

}

void SensorData::initPersistFields()
{
   Parent::initPersistFields();


   addGroup("Sensor");

      addField("effectiveRadius", TypeF32, Offset(mEffectiveRadius, SensorData),
         "@brief Defines the radius that the sensors of a given object is effective out to.");
      addField("penetratingTypeMasks", TypeS32, Offset(mPenetratingTypeMasks, SensorData),
         "@brief Defines what type masks are ignored when performing coverage tests.");

   endGroup("Sensor");
}

bool SensorData::onAdd()
{
   if (!Parent::onAdd())
   {
      return false;
   }

   // TODO: Perform basic validation
   return true;
}

void SensorData::packData(BitStream* stream)
{
   Parent::packData(stream);

   stream->writeFloat(mEffectiveRadius, 32);
   stream->writeSignedInt(mPenetratingTypeMasks, 32);
}

void SensorData::unpackData(BitStream* stream)
{
   Parent::unpackData(stream);

   mEffectiveRadius = stream->readFloat(32);
   mPenetratingTypeMasks = stream->readSignedFloat(32);
}

void SensorData::inspectPostApply()
{
   Parent::inspectPostApply();

   // TODO: Perform basic validation
}
