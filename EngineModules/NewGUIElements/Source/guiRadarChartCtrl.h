#pragma once

#ifndef _GUIBITMAPCTRL_H_
#include "gui/core/guiControl.h"
#endif

#include "gfxDrawFunctions.h"

class GuiRadarChartCtrlProfile;

class GuiRadarChartCtrl : public GuiControl
{
public:

    typedef GuiControl Parent;

protected:
    F32 mRotation;

    enum RadarChartConstants {
        maxSides = 16,
    };

    F32 mVertexMaxValues[maxSides];
    F32 mVertexValues[maxSides];
    ColorI mVertexColors[maxSides];
    StringTableEntry mVertexTexts[maxSides];

public:

    GuiRadarChartCtrl();
    static void initPersistFields();

    void onRender(Point2I offset, const RectI &updateRect);

    DECLARE_CONOBJECT( GuiRadarChartCtrl );
    DECLARE_CATEGORY( "Gui Extended" );
    DECLARE_DESCRIPTION( "A control that indicates relative strengths on a polygon." );
};

class GuiRadarChartCtrlProfile : public GuiControlProfile
{
    typedef GuiControlProfile Parent;

public:
    S32 mNumberOfSides;
    ColorI mOuterColor;
    ColorI mInnerColor;
    F32 mOuterWidth;
    bool mShowValues;
    F32 mChartDivisor;

    /// Name of the bitmap used for drawing the outer border. If not specified, a colored line is drawn.
    DECLARE_IMAGEASSET(GuiRadarChartCtrlProfile, OuterBitmap, onOuterImageChanged, GFXDefaultGUIProfile);
    DECLARE_ASSET_SETGET(GuiRadarChartCtrlProfile, OuterBitmap);

    DECLARE_IMAGEASSET(GuiRadarChartCtrlProfile, InnerBitmap, onInnerImageChanged, GFXDefaultGUIProfile);
    DECLARE_ASSET_SETGET(GuiRadarChartCtrlProfile, InnerBitmap);

    DECLARE_IMAGEASSET(GuiRadarChartCtrlProfile, DataSetBitmap, onDataSetImageChanged, GFXDefaultGUIProfile);
    DECLARE_ASSET_SETGET(GuiRadarChartCtrlProfile, DataSetBitmap);

    void onOuterImageChanged() {}
    void onInnerImageChanged() {}
    void onDataSetImageChanged() {}


    GuiRadarChartCtrlProfile();

    DECLARE_CONOBJECT(GuiRadarChartCtrlProfile);

    /// Initializes fields accessible through the console.
    static void initPersistFields();
};
