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

public:
    Point2F mPanGoal, mCurPan;
    F32     mZoomGoal, mCurZoom;

    GuiCommanderHud();

    bool processCameraQuery(CameraQuery *query);
    void renderWorld(const RectI &updateRect);

    void onRender( Point2I, const RectI &);
    static void initPersistFields();

DECLARE_CONOBJECT( GuiCommanderHud );
};


//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT( GuiCommanderHud );

GuiCommanderHud::GuiCommanderHud()
        :   mPanSpeed(10, 10), mZoomSpeed(1), mCurPan(0,0), mCurZoom(M_PI_F/2),
            mPanGoal(0,0), mZoomGoal(M_PI_F/2), mLastRenderTime(0)
{
}

void GuiCommanderHud::initPersistFields()
{
    Parent::initPersistFields();

    addField("panSpeed",  TypePoint2F, Offset(mPanSpeed,  GuiCommanderHud), "Set the speed (x/y) we pan to our goal.");
    addField("zoomSpeed", TypeF32,     Offset(mZoomSpeed, GuiCommanderHud), "Set the speed we zoom with to our goal.");
}

bool GuiCommanderHud::processCameraQuery(CameraQuery *q)
{
    // Scale ranges based on the highest/lowest point in the terrain
    F32 maxHi,minHi;
    minHi = INFINITY;
    maxHi = -INFINITY;

    // Enumerate all terrains and look for the min/max heights
    F32 currentMinimumHeight;
    F32 currentMaximumHeight;
    const Vector<SceneObject*>& terrains = gClientSceneGraph->getContainer()->getTerrains();
    for (U32 iteration = 0; iteration < terrains.size(); ++iteration)
    {
       TerrainBlock* terrain = dynamic_cast<TerrainBlock*>(terrains[iteration]);
       AssertFatal(terrain, "Must have a list of valid TerrainBlock entries!");

       terrain->getMinMaxHeight(&currentMinimumHeight, &currentMaximumHeight);

       if (currentMinimumHeight < minHi)
       {
          minHi = currentMinimumHeight;
       }

       if (currentMaximumHeight > maxHi)
       {
          maxHi = currentMaximumHeight;
       }
    }

    q->object = NULL;
    q->nearPlane = 1;
    q->farPlane  = mFabs(maxHi) + mFabs(minHi);
    q->fov       = mCurZoom;

    // Make us high up, facing straight down.
    q->cameraMatrix = MatrixF(EulerF(3.14/2, 0, 0)); // rotate us to look straight down
    q->cameraMatrix.setPosition(Point3F(mCurPan.x,mCurPan.y, maxHi + 100)); // and high enough we won't clip

    return true;
}

void GuiCommanderHud::renderWorld(const RectI &updateRect)
{
    // Render (stolen from GameRenderWorld)
    PROFILE_START(GameRenderCommanderWorld);
    FrameAllocator::setWaterMark(0);

    // If you want to render other things, change this mask.
    gClientSceneGraph->renderScene(SPT_Diffuse, EnvironmentObjectType | TerrainObjectType | WaterObjectType);

    // renderScene leaves some states dirty, which causes problems if GameTSCtrl is the last Gui object rendered
    GFX->updateStates();

    AssertFatal(FrameAllocator::getWaterMark() == 0,
                "Error, someone didn't reset the water mark on the frame allocator!");
    FrameAllocator::setWaterMark(0);

    PROFILE_END();
}

void GuiCommanderHud::onRender(Point2I offset, const RectI &updateRect)
{
    if(!processCameraQuery(&mLastCameraQuery))
    {
        // We have no camera, but render the GUI children
        // anyway.  This makes editing GuiTSCtrl derived
        // controls easier in the GuiEditor.
        renderChildControls( offset, updateRect );
        return;
    }

    // set up the camera and viewport stuff:
    F32 wwidth;
    F32 wheight;
    F32 aspectRatio = F32(getWidth()) / F32(getHeight());

    // Use the FOV to calculate the viewport height scale
    // then generate the width scale from the aspect ratio.
    if(!mLastCameraQuery.ortho)
    {
        wheight = mLastCameraQuery.nearPlane * mTan(mLastCameraQuery.fov / 2.0f);
        wwidth = aspectRatio * wheight;
    }
    else
    {
        wheight = mLastCameraQuery.fov;
        wwidth = aspectRatio * wheight;
    }

    F32 hscale = wwidth * 2.0f / F32(getWidth());
    F32 vscale = wheight * 2.0f / F32(getHeight());

    F32 left = (updateRect.point.x - offset.x) * hscale - wwidth;
    F32 right = (updateRect.point.x + updateRect.extent.x - offset.x) * hscale - wwidth;
    F32 top = wheight - vscale * (updateRect.point.y - offset.y);
    F32 bottom = wheight - vscale * (updateRect.point.y + updateRect.extent.y - offset.y);

    RectI tempRect = updateRect;

#ifdef TORQUE_OS_MAC
    Point2I screensize = getRoot()->getWindowSize();
    tempRect.point.y = screensize.y - (tempRect.point.y + tempRect.extent.y);
#endif

    GFX->setViewport( tempRect );

    // Clear the zBuffer so GUI doesn't hose object rendering accidentally
    GFX->clear( GFXClearZBuffer , ColorI(20,20,20), 1.0f, 0 );

    if(!mLastCameraQuery.ortho)
    {
        GFX->setFrustum( left, right, bottom, top,
                         mLastCameraQuery.nearPlane, mLastCameraQuery.farPlane );
    }
    else
    {
        GFX->setOrtho(left, right, bottom, top, mLastCameraQuery.nearPlane, mLastCameraQuery.farPlane, true);

        mOrthoWidth = right - left;
        mOrthoHeight = top - bottom;
    }

    // We're going to be displaying this render at size of this control in
    // pixels - let the scene know so that it can calculate e.g. reflections
    // correctly for that final display result.
    gClientSceneGraph->setDisplayTargetResolution(getExtent());

    // save the world matrix before attempting to draw anything
    GFX->pushWorldMatrix();

    // Set the GFX world matrix to the world-to-camera transform, but don't
    // change the cameraMatrix in mLastCameraQuery. This is because
    // mLastCameraQuery.cameraMatrix is supposed to contain the camera-to-world
    // transform. In-place invert would save a copy but mess up any GUIs that
    // depend on that value.
    MatrixF worldToCamera = mLastCameraQuery.cameraMatrix;
    worldToCamera.inverse();
    GFX->setWorldMatrix( worldToCamera );

    mSaveProjection = GFX->getProjectionMatrix();
    mSaveModelview = GFX->getWorldMatrix();
    mSaveViewport = updateRect;
    mSaveWorldToScreenScale = GFX->getWorldToScreenScale();
    mSaveFrustum = GFX->getFrustum();
    mSaveFrustum.setTransform( mLastCameraQuery.cameraMatrix );

    // Set the default non-clip projection as some
    // objects depend on this even in non-reflect cases.
    gClientSceneGraph->setNonClipProjection( mSaveProjection );

    // restore the world matrix so the GUI will render correctly
    GFX->popWorldMatrix();

    // Update pan/zoom
    S32 time = Platform::getVirtualMilliseconds();
    S32 dt = time - mLastRenderTime;
    mLastRenderTime = time;

    mCurPan  += (mPanGoal  - mCurPan)  * (F32)dt/1000.f;
    mCurZoom += (mZoomGoal - mCurZoom) * (F32)dt/1000.f;

    // Render the world...
    Parent::onRender(offset, updateRect);

    // If you wanted to render custom GUI elements, like a sensor map, icons for
    // players/vehicles/objectives, you would do it here by calling project()
    // for all their positions and drawing bitmaps at the appropriate locations.
}

DefineEngineMethod(GuiCommanderHud, pan, void, (F32 x, F32 y), , "(x, y) Cut to a location.")
{
   object->mPanGoal.set(x, y);
   object->mCurPan.set(x, y);
}

DefineEngineMethod(GuiCommanderHud, panTo, void, (F32 x, F32 y), , "(x, y) Smoothly pan to a location.")
{
   object->mPanGoal.set(x, y);
}

DefineEngineMethod(GuiCommanderHud, zoom, void, (F32 zoom), , "(val) Zoom to a specified level.")
{
   object->mZoomGoal = zoom;
   object->mCurZoom = zoom;
}

DefineEngineMethod(GuiCommanderHud, zoomTo, void, (F32 zoom), , "(val) Smoothly zoom to a specified level.")
{
   object->mZoomGoal = zoom;
}

DefineEngineMethod(GuiCommanderHud, zoomToArea, void, (F32 top, F32 left, F32 right, F32 bottom, bool cut), , "(top, left, right, bottom, bool cut) Smoothly zoom to view the specified area. If cut is set, we jump there.")
{
   // Figure out the center of the area
   Point2F center;

   center.x = (left + right) * 0.5f;
   center.y = (top + bottom) * 0.5f;

   object->mZoomGoal = mFabs(left - right) / 200; // Cheesy scaling fakery.

   // And set our motion
   object->mPanGoal = center;

   // Cut if requested
   if (cut)
   {
      object->mCurPan  = object->mPanGoal;
      object->mCurZoom = object->mZoomGoal;
   }
}
