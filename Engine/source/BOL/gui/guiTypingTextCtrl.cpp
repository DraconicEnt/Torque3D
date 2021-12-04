#include "platform/platform.h"
#include "BOL/gui/guiTypingTextCtrl.h"

#include "gui/core/guiDefaultControlRender.h"
#include "console/consoleTypes.h"
#include "console/console.h"
#include "core/color.h"
#include "i18n/lang.h"
#include "gfx/gfxDrawUtil.h"
#include "console/engineAPI.h"

IMPLEMENT_CONOBJECT( GuiTypingTextCtrl );

ConsoleDocClass( GuiTypingTextCtrl,
   "@brief GUI control object this displays a single line of text, without TorqueML. It will then type the text"
   "out at the specified speed.\n\n"

   "@tsexample\n"
   "  new GuiTypingTextCtrl()\n"
   "  {\n"
   "     text = \"Hello World\";\n"
   "     textID = \"\"STR_HELLO\"\";\n"
   "     maxlength = \"1024\";\n"
   "     typingSpeed = 0.005; // Characters per 32ms\n"
   "      //Properties not specific to this control have been omitted from this example.\n"
   "  };\n"
   "@endtsexample\n\n"

   "@see GuiControl\n"
   "@see Localization\n\n"
   "@ingroup GuiCore\n"
);

GuiTypingTextCtrl::GuiTypingTextCtrl() : GuiTextCtrl()
{
    mTypingSpeed = 0.05f;
    mRenderedText = NULL;
    mRenderedTextLength = 0;
    mRenderedTextProgress = 0.0f;
}

void GuiTypingTextCtrl::initPersistFields()
{
    addField("typingSpeed", TypeF32, Offset(mTypingSpeed, GuiTypingTextCtrl), "How quickly to type the string out in characters per 32ms..");

    Parent::initPersistFields();
}

void GuiTypingTextCtrl::setText(const char *txt)
{
    // Free existing text if present
    if (mRenderedText)
    {
        dFree(mRenderedText);
        mRenderedText = NULL;
    }

    mRenderedTextProgress = 0.0f;

    if (txt)
    {
        mRenderedTextLength = dStrlen(txt);

        mRenderedText = reinterpret_cast<char*>(dMalloc(sizeof(char) * mRenderedTextLength));
        dMemset(mRenderedText, 0x00, sizeof(char) * mRenderedTextLength);
    }

    Parent::setText(txt);
}

void GuiTypingTextCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    renderBorder(RectI(offset, getExtent()), mProfile);

    if (mRenderedText)
    {
        GFX->getDrawUtil()->setBitmapModulation(mProfile->mFontColor);
        renderJustifiedText(offset, getExtent(), (char *) mRenderedText);
    }

    //render the child controls
    renderChildControls(offset, updateRect);
}

void GuiTypingTextCtrl::interpolateTick( F32 delta )
{

}

void GuiTypingTextCtrl::processTick()
{
    if (mRenderedText)
    {
        mRenderedTextProgress += mTypingSpeed;

        // Cap at input text length
        if (mRenderedTextProgress >= mRenderedTextLength)
        {
            mRenderedTextProgress = mRenderedTextLength;
        }

        // Cap at max display length of the child class
        if (mRenderedTextProgress >= MAX_STRING_LENGTH)
        {
            mRenderedTextProgress = MAX_STRING_LENGTH;
        }

        // Copy bytes over
        const dsize_t nextIndex = static_cast<dsize_t>(mRenderedTextProgress);
        dMemcpy(mRenderedText, mText, nextIndex * sizeof(char));
    }
}

void GuiTypingTextCtrl::advanceTime( F32 timeDelta )
{

}
