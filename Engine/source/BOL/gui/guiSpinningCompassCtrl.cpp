#include "platform/platform.h"
#include "BOL/gui/guiSpinningCompassCtrl.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "T3D/gameBase/gameConnection.h"

IMPLEMENT_CONOBJECT(GuiSpinningCompassCtrl);

ConsoleDocClass( GuiSpinningCompassCtrl,
   "@brief A gui control that aligns its rotation to indicate the heading of the current control object.\n\n"
);

GuiSpinningCompassCtrl::GuiSpinningCompassCtrl() : GuiBitmapCtrl()
{

}

void GuiSpinningCompassCtrl::initPersistFields()
{
    Parent::initPersistFields();
}

void GuiSpinningCompassCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    // Update angle first
    GameConnection* connection = GameConnection::getConnectionToServer();
    if (connection)
    {
        GameBase* controlObject = connection->getControlObject();

        if (controlObject)
        {
            const MatrixF &transform = controlObject->getWorldTransform();
            mAngle = mRadToDeg(transform.toEuler().z);
        }
    }

    Parent::onRender(offset, updateRect);
}