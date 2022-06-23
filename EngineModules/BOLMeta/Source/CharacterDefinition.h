#pragma once

// FIXME: Find DECLARE_CONOBJECT and friends and include them here
#ifndef _GUITSCONTROL_H_
#include "gui/3d/guiTSControl.h"
#endif

#ifndef _SCRIPTOBJECTS_H_
#include "console/scriptObjects.h"
#endif

class CharacterDefinition : public ScriptObject
{
    public:
        typedef ScriptObject Parent;

    protected:

    public:        
        /// Name of the bitmap file.  If this is 'texhandle' the bitmap is not loaded
        /// from a file but rather set explicitly on the control.
        DECLARE_IMAGEASSET(CharacterDefinition, TestBitMap, onImageChanged, GFXDefaultGUIProfile);
        DECLARE_ASSET_SETGET(CharacterDefinition, TestBitMap);

        void onImageChanged() {}

        CharacterDefinition();
        static void initPersistFields();

        DECLARE_CONOBJECT( CharacterDefinition );
        DECLARE_CATEGORY( "Gui Extended" );
        DECLARE_DESCRIPTION( "A control used to indicate dialog between characters." );
};
