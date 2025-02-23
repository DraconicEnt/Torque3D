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

#include "T3D/aiConnection.h"
#include "console/engineAPI.h"
#include "T3D/vehicles/hoverVehicle.h"

#ifndef _MOVELIST_H_
#include "T3D/gameBase/moveList.h"
#endif

IMPLEMENT_CONOBJECT( AIConnection );

ConsoleDocClass( AIConnection,
   "@brief Special client connection driven by an AI, rather than a human.\n\n"

   "Unlike other net connections, AIConnection is intended to run unmanned. Rather than "
   "gathering input from a human using a device, move events, triggers, and look events "
   "are driven through functions like AIConnection::setMove.\n\n"

   "In addition to having its own set of functions for managing client move events, "
   "a member variable inherited by GameConnection is toggle: mAIControlled. This is useful "
   "for a server to determine if a connection is AI driven via the function GameConnection::isAIControlled\n\n"

   "AIConnection is an alternative to manually creating an AI driven game object. When you "
   "want the server to manage AI, you will create a specific one from script using a "
   "class like AIPlayer. If you do not want the server managing the AI and wish to simulate "
   "a complete client connection, you will use AIConnection\n\n."

   "To get more specific, if you want a strong alternative to AIPlayer (and wish to make use "
   "of the AIConnection structure), consider AIClient. AIClient inherits from AIConnection, "
   "contains quite a bit of functionality you will find in AIPlayer, and has its own Player "
   "object.\n\n"

   "@tsexample\n"
   "// Create a new AI client connection\n"
   "%botConnection = aiConnect(\"MasterBlaster\" @ %i, -1, 0.5, false, \"SDF\", 1.0);\n\n"
   "// In another area of the code, you can locate this and any other AIConnections\n"
   "// using the isAIControlled function\n"
   "for(%i = 0; %i < ClientGroup.getCount(); %i++)\n"
   "{\n"
   "   %client = ClientGroup.getObject(%i);\n"
   "   if(%client.isAIControlled())\n"
   "   {\n"
   "      // React to this AI controlled client\n"
   "   }\n"
   "}\n"
   "@endtsexample\n\n"

   "@note This is a legacy class, which you are discouraged from using as it will "
   "most likely be deprecated in a future version. For now it has been left in for "
   "backwards compatibility with TGE and the old RTS Kit. Use GameConnection "
   "and AIPlayer instead.\n\n"

   "@see GameConnection, NetConnection, AIClient\n\n"

   "@ingroup AI\n"
   "@ingroup Networking\n"
);
//-----------------------------------------------------------------------------

AIConnection::AIConnection() {
   mAIControlled = true;
   mMove = NullMove;
}


//-----------------------------------------------------------------------------

void AIConnection::clearMoves( U32 )
{
   // Clear the pending move list. This connection generates moves
   // on the fly, so there are never any pending moves.
}

void AIConnection::setMove(Move* m)
{
   mMove = *m;
}

const Move& AIConnection::getMove()
{
   return mMove;
}

/// Retrive the pending moves
/**
 * The GameConnection base class queues moves for delivery to the
 * controll object.  This function is normally used to retrieve the
 * queued moves recieved from the client.  The AI connection does not
 * have a connected client and simply generates moves on-the-fly
 * base on it's current state.
 */
U32 AIConnection::getMoveList( Move **lngMove, U32 *numMoves )
{
   *numMoves = 1;
   *lngMove = &mMove;
   return *numMoves;
}


void AIConnection::interpolateTick(F32 delta)
{

}

void AIConnection::processTick()
{
   // Reset move state
   mMove = NullMove;

   GameBase* controlObject = getControlObject();
   if (controlObject)
   {
      if (controlObject->getClassRep()->getClassName() == HoverVehicle::dynClassRep.getClassName())
      {
         Con::errorf("Controlling hover!");
      }
   }

   // FIXME: Gen moves here based on object type
   mMoveList->pushMove(mMove);
}

void AIConnection::advanceTime(F32 timeDelta)
{

}

void AIConnection::setAimLocation(const Point3F& location)
{
   mAimLocation = location;
}


//-----------------------------------------------------------------------------
// Console functions & methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

static inline F32 moveClamp(F32 v)
{
   // Support function to convert/clamp the input into a move rotation
   // which only allows 0 -> M_2PI.
   F32 a = mClampF(v, -M_2PI_F, M_2PI_F);
   return (a < 0) ? a + M_2PI_F : a;
}


//-----------------------------------------------------------------------------
/// Construct and connect an AI connection object
DefineEngineStringlyVariadicFunction(aiConnect, S32 , 2, 20, "(...)"
   "@brief Creates a new AIConnection, and passes arguments to its onConnect script callback.\n\n"
   "@returns The newly created AIConnection\n"
   "@see GameConnection for parameter information\n"
   "@ingroup AI")
{
   // Create the connection
   AIConnection *aiConnection = new AIConnection();
   aiConnection->registerObject();

   // Add the connection to the client group
   SimGroup *g = Sim::getClientGroup();
   g->addObject( aiConnection );

   // Prep the arguments for the console exec...
   // Make sure and leav args[1] empty.
   const char* args[21];
   args[0] = "onConnect";
   args[1] = NULL; // Filled in later
   for (S32 i = 1; i < argc; i++)
      args[i + 1] = argv[i];

   // Execute the connect console function, this is the same
   // onConnect function invoked for normal client connections
   Con::execute(aiConnection, argc + 1, args);
   return aiConnection->getId();
}


//-----------------------------------------------------------------------------
DefineEngineMethod(AIConnection, aimAtLocation, void, (Point3F location), ,"(Point3F location)"
              "Requests that the AI aim at a specific location.\n\n"
              "@param   location A vector representing the position to aim at.\n")
{
   object->setAimLocation(location);
}

//-----------------------------------------------------------------------------

DefineEngineMethod(AIConnection,getAddress,const char*,(), ,"")
{
   // Override the netConnection method to return to indicate
   // this is an ai connection.
   return "ai:local";
}

