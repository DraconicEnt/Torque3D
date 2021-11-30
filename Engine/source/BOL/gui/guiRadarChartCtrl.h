#pragma once

#ifndef _GUIBITMAPCTRL_H_
#include "gui/core/guiControl.h"
#endif

class GuiRadarChartCtrl : public GuiControl
{
public:

    typedef GuiControl Parent;

protected:
    U32 mNumberOfSides;
    F32 mRotation;
    ColorI mOuterColor;
    F32 mOuterWidth;

    /// Name of the bitmap used for drawing the outer border. If not specified, a colored line is drawn.
    DECLARE_IMAGEASSET(GuiRadarChartCtrl, OuterBitmap, onOuterImageChanged, GFXDefaultGUIProfile);
    DECLARE_ASSET_SETGET(GuiRadarChartCtrl, OuterBitmap);

    void onOuterImageChanged() {}

public:

    GuiRadarChartCtrl();
    static void initPersistFields();

    void onRender(Point2I offset, const RectI &updateRect);

    bool onWake();
    void onSleep();

DECLARE_CONOBJECT( GuiRadarChartCtrl );
    DECLARE_CATEGORY( "Gui Images" );
    DECLARE_DESCRIPTION( "A control that indicates relative strengths on a polygon." );
};
