#pragma once

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif
#ifndef _SIMDATABLOCK_H_
#include "console/simDatablock.h"
#endif
#ifndef _BITSTREAM_H_
#include "core/stream/bitStream.h"
#endif

class SensorData : public SimDataBlock
{
   typedef SimDataBlock Parent;

public:
   SensorData();
   ~SensorData();

   DECLARE_CONOBJECT(SensorData);
   static void initPersistFields();

   // SimDataBlock.
   virtual bool onAdd();
   virtual void packData(BitStream* stream);
   virtual void unpackData(BitStream* stream);
   virtual void inspectPostApply();

public:
   F32 mEffectiveRadius;
   S32 mPenetratingTypeMasks;
};
