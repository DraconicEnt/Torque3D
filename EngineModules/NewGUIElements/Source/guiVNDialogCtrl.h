#pragma once

#include <vector>
#include <unordered_map>

#ifndef _SIMSET_H_
#include "console/simSet.h"
#endif

#ifndef _GUITSCONTROL_H_
#include "gui/3d/guiTSControl.h"
#endif

#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif

#include "CharacterDefinition.h"

class GuiVNDialogCharacterHandle;

class GuiVNDialogCommand
{
    protected:
        GuiVNDialogCharacterHandle* mCharacterHandle;

    public:
        GuiVNDialogCommand(GuiVNDialogCharacterHandle* handle);

        virtual bool getIsComplete() = 0;
        virtual void advanceTime(float deltaTime) = 0;
};

class GuiVNDialogMoveCommand : public GuiVNDialogCommand
{
    protected:
        const Point3F mStartSceneCoordinate;
        const Point3F mTargetSceneCoordinate;

        const F32 mSpeed;

        F32 mProgress;

        // void interpolate(const Point3F&, const Point3F&, F32);
    public:
        GuiVNDialogMoveCommand(GuiVNDialogCharacterHandle* handle, const Point3F& startSceneCoordinate, const Point3F& targetSceneCoordinate, F32 speed);
        void advanceTime(float deltaTime);
        bool getIsComplete();
};

class GuiVNDialogCharacterHandle
{
    public:
        Point3F mSceneCoordinate;

        GuiVNDialogCharacterHandle(CharacterDefinition* character);

        CharacterDefinition* getCharacter();

        void advanceTime(float deltaTime);
        void addCommand(GuiVNDialogCommand* command);

    private:
        std::vector<GuiVNDialogCommand*> mCommands;
        CharacterDefinition* mCharacter;
};

class GuiVNDialogCtrl : public GuiTSCtrl, public ITickable
{
    public:
        typedef GuiTSCtrl Parent;

    private:
        S32 mNextCharacterID;

    protected:
        F32 mSceneDistance;
        F32 mFOV;

        GFXStateBlockRef mBlendState;

        void renderQuad(GFXTextureObject* texture, const MatrixF& matrix);
        Point3F getCameraLeftEdge(F32 distance);
        Point3F getCameraRightEdge(F32 distance);

        Point3F cameraAngleToWorldPosition(F32 distance, F32 angle);

        std::unordered_map<S32, GuiVNDialogCharacterHandle*> mCharacters;

    public:
        GuiVNDialogCtrl();
        static void initPersistFields();

        //void onRender(Point2I offset, const RectI &updateRect);

        S32 addCharacter(CharacterDefinition* character);
        bool removeCharacter(CharacterDefinition* character);

        virtual void renderWorld(const RectI &updateRect) override;

        virtual bool processCameraQuery(CameraQuery *camq) override;

        virtual void interpolateTick( F32 delta );

        virtual void processTick();

        virtual void advanceTime( F32 timeDelta );

        virtual void onDeleteNotify(SimObject *object) override;

        S32 findCharacter(CharacterDefinition* character);

        GuiVNDialogCharacterHandle* getCharacterHandle(S32 id);


        DECLARE_CONOBJECT( GuiVNDialogCtrl );
        DECLARE_CATEGORY( "Gui Extended" );
        DECLARE_DESCRIPTION( "A control used to indicate dialog between characters." );
};
