//-----------------------------------------------------------------------------
// Commander Map HUD
//
// Portions Copyright (c) GarageGames.Com
// Copyright (c) Ben Garney
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "gui/3d/guiTSControl.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "gui/core/guiCanvas.h"
#include "gui/editor/guiShapeEdPreview.h"
#include "renderInstance/renderPassManager.h"
#include "lighting/lightManager.h"
#include "lighting/lightInfo.h"
#include "core/resourceManager.h"
#include "scene/sceneManager.h"
#include "scene/sceneRenderState.h"
#include "gfx/primBuilder.h"
#include "gfx/gfxDrawUtil.h"
#include "terrain/terrData.h"

//-----------------------------------------------------------------------------

class GuiCommanderHud : public GuiTSCtrl
{
private:
    typedef GuiTSCtrl Parent;

    Point2F mPanSpeed;
    F32     mZoomSpeed;
    S32     mLastRenderTime;
    ColorI mBackgroundColor;

    /// Whether or not the user is currently dragging in the Hud
    bool mDragging;

    /// Whether or not the user is currently panning in the hud
    bool mPanning;


    Point2I mCursorLastPosition;
    Point2I mDragStart;
    Point2I mDragCurrent;

public:
    Point2F mPanGoal, mCurPan;
    F32     mZoomGoal, mCurZoom;

    GuiCommanderHud();

    void onMouseUp(const GuiEvent &event);
    void onMouseDown(const GuiEvent &event);
    void onRightMouseDragged(const GuiEvent& event);
    void onRightMouseDown(const GuiEvent& event);
    void onRightMouseUp(const GuiEvent& event);
    void onMouseDragged(const GuiEvent &event);
    bool onMouseWheelDown(const GuiEvent& event);
    bool onMouseWheelUp(const GuiEvent& event);

    bool processCameraQuery(CameraQuery *query);
    void renderWorld(const RectI &updateRect);

    void onRender( Point2I, const RectI &);
    static void initPersistFields();

    DECLARE_CONOBJECT( GuiCommanderHud );
};
