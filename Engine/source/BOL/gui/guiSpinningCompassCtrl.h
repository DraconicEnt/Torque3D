#pragma once

#ifndef _GUIBITMAPCTRL_H_
#include "gui/controls/guiBitmapCtrl.h"
#endif

class GuiSpinningCompassCtrl : public GuiBitmapCtrl
{
   public:
   
      typedef GuiBitmapCtrl Parent;

   public:
      
      GuiSpinningCompassCtrl();
      static void initPersistFields();

      void onRender(Point2I offset, const RectI &updateRect);

      DECLARE_CONOBJECT( GuiSpinningCompassCtrl );
      DECLARE_CATEGORY( "Gui Images" );
      DECLARE_DESCRIPTION( "A control that auto-rotates to indicate the current control object heading." );
};
