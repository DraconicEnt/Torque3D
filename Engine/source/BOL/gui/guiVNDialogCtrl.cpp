#include "platform/platform.h"
#include "BOL/gui/guiVNDialogCtrl.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "T3D/gameBase/gameConnection.h"

IMPLEMENT_CONOBJECT(GuiVNDialogCtrl);

ConsoleDocClass( GuiVNDialogCtrl,
   "@brief A gui control that is used to indicate dialog between characters.\n\n"
);

GuiVNDialogCtrl::GuiVNDialogCtrl() : GuiContainer()
{
    INIT_ASSET(TestBitMap);

    mDepth = 0.0f;
}

void GuiVNDialogCtrl::initPersistFields()
{
    INITPERSISTFIELD_IMAGEASSET(TestBitMap, GuiVNDialogCtrl, The bitmap file to display in the control);
    addField("depth", TypeF32, Offset(mDepth, GuiVNDialogCtrl), "Test");
    Parent::initPersistFields();
}

void GuiVNDialogCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    Parent::onRender(offset, updateRect);

    if (mTestBitMap)
    {
        MatrixF matrix(EulerF(0.0f, 0.0f, 0.0f));
        matrix.scale(Point3F(mTestBitMap->getWidth(), mTestBitMap->getHeight(), 1.0f));
        matrix.setPosition(Point3F(offset.x, offset.y, 0.0f));

        GFX->pushWorldMatrix();
        GFX->multWorld(matrix);

        GFXStateBlockDesc description;
        description.setAlphaTest(true);
        description.setBlend(true, GFXBlendSrcAlpha, GFXBlendInvSrcAlpha);
        description.setZReadWrite(true, false);
        description.vertexColorEnable = true;
        description.samplersDefined = true;
        description.samplers[0] = GFXSamplerStateDesc::getWrapLinear();

        // Setup the vertices of the pane to draw on
        GFXVertexBufferHandle<GFXVertexPCT> vertices(GFX, 4, GFXBufferTypeVolatile);
        vertices.lock();

      //  vertices[0].point.set(offset.x, offset.y, 0.0f);
       // vertices[1].point.set(offset.x + getExtent().x, offset.y, 0.0f);
       // vertices[2].point.set(offset.x, offset.y + getExtent().y, 0.0f);
       // vertices[3].point.set(offset.x + getExtent().x, offset.y + getExtent().y, 0.0f);

        vertices[0].point.set(Point3F(0.0f, 0.0f, 0.0f));
        vertices[1].point.set(Point3F(1.0f, 0.0f, 0.0f));
        vertices[2].point.set(Point3F(0.0f, 1.0f, 0.0f));
        vertices[3].point.set(Point3F(1.0f, 1.0f, 0.0f));

        vertices[0].texCoord.set(-1.0f, -1.0f);
        vertices[1].texCoord.set(1.0f, -1.0f);
        vertices[2].texCoord.set(-1.0f, 1.0f);
        vertices[3].texCoord.set(1.0f, 1.0f);

        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = ColorI::WHITE;

        vertices.unlock();

        GFX->setVertexBuffer(vertices);
        GFX->setStateBlockByDesc( description );

        GFX->setTexture(0, mTestBitMap);
        GFX->setupGenericShaders(GFXDevice::GSModColorTexture);

        GFX->drawPrimitive(GFXTriangleStrip, 0, 2);

        GFX->popWorldMatrix();
    }

    renderChildControls(offset, updateRect);
}

void GuiVNDialogCtrl::interpolateTick( F32 delta )
{

}

void GuiVNDialogCtrl::processTick()
{

}

void GuiVNDialogCtrl::advanceTime( F32 timeDelta )
{

}
