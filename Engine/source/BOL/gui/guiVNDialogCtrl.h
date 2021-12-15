#pragma once

#ifndef _GUICONTAINER_H_
#include "gui/containers/guiContainer.h"
#endif

#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif

class GuiVNDialogCtrl : public GuiContainer, public ITickable
{
    public:
        typedef GuiContainer Parent;

    protected:
        /// Name of the bitmap file.  If this is 'texhandle' the bitmap is not loaded
        /// from a file but rather set explicitly on the control.
        DECLARE_IMAGEASSET(GuiVNDialogCtrl, TestBitMap, onImageChanged, GFXDefaultGUIProfile);
        DECLARE_ASSET_SETGET(GuiVNDialogCtrl, TestBitMap);

        void onImageChanged() {}

        F32 mDepth;

    public:
        GuiVNDialogCtrl();
        static void initPersistFields();

        void onRender(Point2I offset, const RectI &updateRect);

        virtual void interpolateTick( F32 delta );

        virtual void processTick();

        virtual void advanceTime( F32 timeDelta );

        DECLARE_CONOBJECT( GuiVNDialogCtrl );
        DECLARE_CATEGORY( "Gui Images" );
        DECLARE_DESCRIPTION( "A control used to indicate dialog between characters." );
};
