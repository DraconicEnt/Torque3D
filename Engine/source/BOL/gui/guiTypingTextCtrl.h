#pragma once

#ifndef _GFONT_H_
#include "gfx/gFont.h"
#endif
#ifndef _GUITYPES_H_
#include "gui/core/guiTypes.h"
#endif
#ifndef _GUITEXTCTRL_H_
#include "gui/controls/guiTextCtrl.h"
#endif
#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif

class GuiTypingTextCtrl : public GuiTextCtrl, public ITickable
{
private:
   typedef GuiTextCtrl Parent;

protected:
    char* mRenderedText;
    F32 mRenderedTextProgress;
    F32 mTypingSpeed;
    dsize_t mRenderedTextLength;

public:

   //creation methods
   DECLARE_CONOBJECT(GuiTypingTextCtrl);
   DECLARE_CATEGORY( "Gui Text" );
   DECLARE_DESCRIPTION( "A control that displays a single line of text." );

   GuiTypingTextCtrl();
   static void initPersistFields();

   virtual void setText(const char *txt = NULL);

   virtual void interpolateTick( F32 delta );

   virtual void processTick();

   virtual void advanceTime( F32 timeDelta );

   //rendering methods
   void onRender(Point2I offset, const RectI &updateRect);
};