//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _AICONNECTION_H_
#define _AICONNECTION_H_

#ifndef _GAMECONNECTION_H_
#include "T3D/gameBase/gameConnection.h"
#endif
#ifndef _MOVEMANAGER_H_
#include "T3D/gameBase/moveManager.h"
#endif

//-----------------------------------------------------------------------------

class AIConnection : public GameConnection, public ITickable
{
   typedef GameConnection Parent;

protected:
   Move mMove;

   Point3F mAimLocation;

public:
   AIConnection();
   DECLARE_CONOBJECT( AIConnection );

   // Interface
   const Move& getMove();
   void setMove(Move *m);

   // GameConnection overrides
   void clearMoves(U32 n);
   virtual U32 getMoveList(Move **,U32 *numMoves);


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

   void setAimLocation(const Point3F& location);
};


#endif
