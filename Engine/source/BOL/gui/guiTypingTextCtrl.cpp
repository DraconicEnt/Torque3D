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

GuiTypingTextCtrl::GuiTypingTextCtrl() : GuiControl()
{
    mSourceText = StringTable->EmptyString();
    mRenderedTextLength = 0;
    mLineIndex = 0;
    mTextBlockIndex = 0;
    mTextBlockProgress = 0.0f;
}

void GuiTypingTextCtrl::initPersistFields()
{
    addProtectedField("text", TypeCaseString, Offset(mSourceText, GuiTypingTextCtrl), setText, getTextProperty,
                      "The text to show on the control.");

    Parent::initPersistFields();
}

void GuiTypingTextCtrl::setText(const char* newText)
{
    mSourceLines.clear();
    mRenderedLines.clear();

    parseTextInput(newText, dStrlen(newText), mSourceLines);

    // Enumerate lines
    for (dsize_t lineIteration = 0; lineIteration < mSourceLines.size(); ++lineIteration)
    {
        const Vector<TextBlockEntry>& currentSourceLine = *(mSourceLines.begin() + lineIteration);

        Vector<TextBlockEntry> renderedLine;

        // Enumerate blocks in the line
        for (dsize_t blockIteration = 0; blockIteration < currentSourceLine.size(); ++blockIteration)
        {
            const TextBlockEntry& currentSourceBlock = *(currentSourceLine.begin() + blockIteration);
            renderedLine.push_back(TextBlockEntry(currentSourceBlock.mTextLength + 1)); // Add NULL padding
        }

        mRenderedLines.push_back(renderedLine);
    }

    // Set source text for script access
    mSourceText = StringTable->insert(newText, true);
}

void GuiTypingTextCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    renderBorder(RectI(offset, getExtent()), mProfile);

    // Enumerate lines
    GFX->getDrawUtil()->setBitmapModulation(mProfile->mFontColor);
    for (dsize_t lineIteration = 0; lineIteration < mSourceLines.size(); ++lineIteration)
    {
        const Vector<TextBlockEntry>& currentSourceLine = *(mSourceLines.begin() + lineIteration);

        // Enumerate blocks in the line
        for (dsize_t blockIteration = 0; blockIteration < currentSourceLine.size(); ++blockIteration)
        {
            const TextBlockEntry& currentSourceBlock = *(currentSourceLine.begin() + blockIteration);

            // FIXME: Draw per line
            renderJustifiedText(offset, getExtent(), (char *) currentSourceBlock.mText);
        }
    }

    //render the child controls
    renderChildControls(offset, updateRect);
}

void GuiTypingTextCtrl::interpolateTick( F32 delta )
{

}

void GuiTypingTextCtrl::processTick()
{
   if (mLineIndex < mSourceLines.size())
   {
       const Vector<TextBlockEntry>& currentLine = *(mSourceLines.begin() + mLineIndex);

       if (mTextBlockIndex < currentLine.size())
       {
           const TextBlockEntry& currentBlock = *(currentLine.begin() + mTextBlockIndex);

           const dsize_t previousTextIndex = static_cast<dsize_t>(mTextBlockProgress);
           mTextBlockProgress += currentBlock.mTypingSpeed;
           const dsize_t nextTextIndex = static_cast<dsize_t>(mTextBlockProgress);

           // Copy into render buffer
           Vector<TextBlockEntry>& renderedLine = *(mRenderedLines.begin() + mLineIndex);
           TextBlockEntry& renderedBlock = *(renderedLine.begin() + mTextBlockIndex);

           dMemcpy(renderedBlock.mText + previousTextIndex, currentBlock.mText + previousTextIndex, nextTextIndex - previousTextIndex);
       }
       else
       {
           ++mLineIndex;
           mTextBlockIndex = 0;
           mTextBlockProgress = 0.0f;
       }
   }
}

void GuiTypingTextCtrl::advanceTime( F32 timeDelta )
{

}

void GuiTypingTextCtrl::parseTextInput(const char* text, const dsize_t textLength, Vector<Vector<TextBlockEntry>>& out)
{
    Vector<TextBlockEntry> currentLine;

    // Enumerate all lines
    dsize_t currentLineStart = 0;
    for (dsize_t iteration = 0; iteration < textLength; ++iteration)
    {
        // If we encounter a newline character, copy the buffer into a new entry
        if (text[iteration] == '\n')
        {
            const char* copyTextPointer = text + currentLineStart;
            currentLine.push_back(TextBlockEntry(copyTextPointer, iteration - currentLineStart));

            out.push_back(currentLine);
            currentLine = Vector<TextBlockEntry>();
            currentLineStart = iteration;
        }
    }

    // Copy remaining buffer into a new line
    currentLine = Vector<TextBlockEntry>();
    if (currentLineStart < textLength)
    {
        const char* copyTextPointer = text + currentLineStart;
        currentLine.push_back(TextBlockEntry(copyTextPointer, textLength - currentLineStart));

        out.push_back(currentLine);
    }
}

