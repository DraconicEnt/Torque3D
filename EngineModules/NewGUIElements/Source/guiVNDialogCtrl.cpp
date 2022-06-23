#include "platform/platform.h"
#include "guiVNDialogCtrl.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "T3D/gameBase/gameConnection.h"

/*
    Test
*/
GuiVNDialogCharacterHandle::GuiVNDialogCharacterHandle(CharacterDefinition* character) : mSceneCoordinate(0.0f, 2.0f, 0.0f), mCharacter(character)
{

}

CharacterDefinition* GuiVNDialogCharacterHandle::getCharacter()
{
    return mCharacter;
}

GuiVNDialogMoveCommand::GuiVNDialogMoveCommand(GuiVNDialogCharacterHandle* handle, const Point3F& startSceneCoordinate, const Point3F& targetSceneCoordinate, F32 speed) : GuiVNDialogCommand(handle), mStartSceneCoordinate(startSceneCoordinate), mTargetSceneCoordinate(targetSceneCoordinate), mSpeed(speed), mProgress(0.0f)
{

}


GuiVNDialogCommand::GuiVNDialogCommand(GuiVNDialogCharacterHandle* handle) : mCharacterHandle(handle)
{

}
    

void GuiVNDialogMoveCommand::advanceTime(float deltaTime)
{
    mProgress += mSpeed * deltaTime;
    if (mProgress >= 1.0f)
    {
        mProgress = 1.0f;
    }

    // Update coordinate
    //void interpolate(const Point3F&, const Point3F&, F32);
    mCharacterHandle->mSceneCoordinate.interpolate(mStartSceneCoordinate, mTargetSceneCoordinate, mProgress);
}

bool GuiVNDialogMoveCommand::getIsComplete()
{
    return mProgress >= 1.0f;
}

void GuiVNDialogCharacterHandle::advanceTime(float deltaTime)
{
    std::vector<U32> removedCommandIndices;

    for (U32 iteration = 0; iteration < mCommands.size(); ++iteration)
    {
        GuiVNDialogCommand* command = mCommands[iteration];

        command->advanceTime(deltaTime);

        if (command->getIsComplete())
        {
            removedCommandIndices.push_back(iteration);
        }
    }

    // Remove commands in reverse order
    for (auto iterator = removedCommandIndices.rbegin(); iterator != removedCommandIndices.rend(); ++iterator)
    {
        delete mCommands[*iterator];
        mCommands.erase(mCommands.begin() + *iterator);
    }
}

void GuiVNDialogCharacterHandle::addCommand(GuiVNDialogCommand* command)
{
    mCommands.push_back(command);
}

IMPLEMENT_CONOBJECT(GuiVNDialogCtrl);

ConsoleDocClass( GuiVNDialogCtrl,
   "@brief A gui control that is used to indicate dialog between characters.\n\n"
);

GuiVNDialogCtrl::GuiVNDialogCtrl() : GuiTSCtrl()
{
    mNextCharacterID = 0;

    mFOV = M_PI / 2.0f;

    mSceneDistance = 2.0f;

    // Setup state block
    GFXStateBlockDesc desc;
    desc.zEnable = true;
    desc.setCullMode(GFXCullNone);
    desc.setBlend(true, GFXBlendSrcAlpha, GFXBlendInvSrcAlpha);
    desc.samplersDefined = true;
    desc.vertexColorEnable = true;

    mBlendState = GFX->createStateBlock(desc);
}

void GuiVNDialogCtrl::initPersistFields()
{
    addField("sceneDistance", TypeF32, Offset(mSceneDistance, GuiVNDialogCtrl), "The distance away from the camera the scene is taking place.");
    addField("fov", TypeF32, Offset(mFOV, GuiVNDialogCtrl), "Camera field of view.");
    Parent::initPersistFields();
}

void GuiVNDialogCtrl::onDeleteNotify(SimObject* object)
{
    removeCharacter((CharacterDefinition*)object);
}

Point3F GuiVNDialogCtrl::getCameraLeftEdge(F32 distance)
{
    return cameraAngleToWorldPosition(distance, -mFOV / 2.0f);
}

Point3F GuiVNDialogCtrl::getCameraRightEdge(F32 distance)
{
    return cameraAngleToWorldPosition(distance, mFOV / 2.0f);
}

S32 GuiVNDialogCtrl::findCharacter(CharacterDefinition* character)
{
    for (auto iterator = mCharacters.begin(); iterator != mCharacters.end(); ++iterator)
    {
        auto keyValue = *iterator;

        if (keyValue.second->getCharacter() == character)
        {
            return keyValue.first;
        }
    }
    return -1;
}

GuiVNDialogCharacterHandle* GuiVNDialogCtrl::getCharacterHandle(S32 id)
{
    auto search = mCharacters.find(id);

    if (search != mCharacters.end())
    {
        return (*search).second;
    }
    return NULL;
}

S32 GuiVNDialogCtrl::addCharacter(CharacterDefinition* character)
{
    if (findCharacter(character) != -1)
    {
        Con::errorf("Characters cannot be added to a GuiVNDialogCtrl multiple times!");
        return -1;
    }

    S32 insertIndex = mCharacters.size();

    deleteNotify(character);
    mCharacters[mNextCharacterID++] = new GuiVNDialogCharacterHandle(character);

    return insertIndex;
}

bool GuiVNDialogCtrl::removeCharacter(CharacterDefinition* character)
{
    S32 search = findCharacter(character);

    if (search != -1)
    {
        delete mCharacters[search];
        mCharacters.erase(search);

        return true;
    }
    return false;
}

Point3F GuiVNDialogCtrl::cameraAngleToWorldPosition(F32 distance, F32 angle)
{
    // Take the current view matrix and rotate it to obtain the correct angle
    const MatrixF viewMatrix = GFX->getViewMatrix();
    const MatrixF rotationMatrix = MatrixF(EulerF(0, 0, angle), Point3F(0, 0, 1));
    const MatrixF finalMatrix = viewMatrix * rotationMatrix;

    return finalMatrix.getForwardVector() * distance;
}

void GuiVNDialogCtrl::renderWorld(const RectI &updateRect)
{
    for (auto iterator = mCharacters.begin(); iterator != mCharacters.end(); ++iterator)
    {
        auto keyValue = *iterator;
        GuiVNDialogCharacterHandle* handle = keyValue.second;

        // Set object matrix
        MatrixF matrix(EulerF(0.0f, 0.0f, 0.0f));
        matrix.scale(Point3F(1.0f, 1.0f, 1.0f));
        matrix.setPosition(cameraAngleToWorldPosition(handle->mSceneCoordinate.y, handle->mSceneCoordinate.x));
        
        renderQuad(handle->getCharacter()->mTestBitMap, matrix);

        GFX->setClipRect(updateRect);
    }
}

void GuiVNDialogCtrl::renderQuad(GFXTextureObject* texture, const MatrixF& matrix)
{
    // FIXME: Can we store this somewhere?
    GFXVertexBufferHandle<GFXVertexPCT> vertices(GFX, 4, GFXBufferTypeVolatile);
    vertices.lock();

    vertices[0].point.set(Point3F(-1, 0, 1));
    vertices[1].point.set(Point3F(1, 0, 1));
    vertices[2].point.set(Point3F(-1, 0, -1));
    vertices[3].point.set(Point3F(1, 0, -1));

    vertices[0].texCoord.set(0, 0);
    vertices[1].texCoord.set(1, 0);
    vertices[2].texCoord.set(0, 1);
    vertices[3].texCoord.set(1, 1);

    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = ColorI::WHITE;

    vertices.unlock();

    GFX->pushWorldMatrix();
    GFX->multWorld(matrix);

    GFX->setVertexBuffer(vertices);
    GFX->setStateBlock(mBlendState);

    GFX->setTexture(0, texture);
    GFX->setupGenericShaders(GFXDevice::GSModColorTexture);
    GFX->drawPrimitive(GFXTriangleStrip, 0, 2);
    
    GFX->popWorldMatrix();
}

void GuiVNDialogCtrl::interpolateTick( F32 delta )
{

}

bool GuiVNDialogCtrl::processCameraQuery(CameraQuery *camq)
{
	// pretty hacky hardcoded values
	// givefs a reasonable viewing angle
	camq->nearPlane = 0.1f; // near clip plane
	camq->farPlane = 20.0f; // far clip plane
	camq->fov = mFOV;     // field of view

    // Ensure camera position and rotation are sane
	MatrixF cam;
	cam.set(EulerF(0, 0, 0.0f)); // rotation
	cam.setColumn(3, Point3F(0, 0, 0)); // position

	camq->cameraMatrix = cam;
	return (true);
}

void GuiVNDialogCtrl::processTick()
{

}

void GuiVNDialogCtrl::advanceTime( F32 timeDelta )
{
    for (auto iterator = mCharacters.begin(); iterator != mCharacters.end(); ++iterator)
    {
        auto keyValue = *iterator;
        GuiVNDialogCharacterHandle* handle = keyValue.second;

        handle->advanceTime(timeDelta);
    }
}

DefineEngineMethod( GuiVNDialogCtrl, addCharacter, S32, (SimObject* inObject),,
   "@brief Adds a character to this VN dialog.\n\n" )
{
    CharacterDefinition* addedCharacter = dynamic_cast<CharacterDefinition*>(inObject);

    if (!addedCharacter)
    {
        Con::errorf("Input must be a valid CharacterDefinition.");
        return -1;
    }

    return object->addCharacter(addedCharacter);
}

DefineEngineMethod( GuiVNDialogCtrl, removeCharacter, S32, (SimObject* inObject),,
   "@brief Removes a character from this VN dialog.\n\n" )
{
    CharacterDefinition* addedCharacter = dynamic_cast<CharacterDefinition*>(inObject);

    if (!addedCharacter)
    {
        Con::errorf("Input must be a valid CharacterDefinition.");
        return -1;
    }

    return object->removeCharacter(addedCharacter);
}

DefineEngineMethod( GuiVNDialogCtrl, setCharacterPosition, bool, (S32 characterID, F32 cameraAngle, F32 cameraDistance),,
   "@brief Adds a character to this VN dialog.\n\n" )
{
    GuiVNDialogCharacterHandle* handle = object->getCharacterHandle(characterID);

    if (handle == NULL)
    {
        Con::errorf("Invalid character handle ID: %u", characterID);
        return false;
    }

    handle->mSceneCoordinate = Point3F(cameraAngle, cameraDistance, 0.0f);
    return false;
}

DefineEngineMethod( GuiVNDialogCtrl, addCharacterMoveCommand, bool, (S32 characterID, F32 cameraAngle, F32 cameraDistance, F32 speed),,
   "@brief Adds a character to this VN dialog.\n\n" )
{
    GuiVNDialogCharacterHandle* handle = object->getCharacterHandle(characterID);

    if (handle == NULL)
    {
        Con::errorf("Invalid character handle ID: %u", characterID);
        return false;
    }

    handle->addCommand(new GuiVNDialogMoveCommand(handle, handle->mSceneCoordinate, Point3F(cameraAngle, cameraDistance, 0.0f), speed));
    return false;
}

/*
DefineEngineMethod( SceneObject, unmount, void, (),,
   "Unmount us from the currently mounted object if any.\n" )
{
   object->unmount();
}
*/