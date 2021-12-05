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
        const std::vector<TextBlockEntry>& currentSourceLine = mSourceLines[lineIteration];

        std::vector<TextBlockEntry> renderedLine;

        // Enumerate blocks in the line
        for (dsize_t blockIteration = 0; blockIteration < currentSourceLine.size(); ++blockIteration)
        {
           const TextBlockEntry& currentSourceBlock = currentSourceLine[blockIteration];
           renderedLine.push_back(TextBlockEntry(currentSourceBlock.mTextLength + 1)); // Add NULL padding
        }

        mRenderedLines.push_back(renderedLine);
    }

    // Set source text for script access
    mSourceText = StringTable->insert(newText, true);

    // Reset draw state
    mTextBlockIndex = 0;
    mLineIndex = 0;
    mTextBlockProgress = 0.0f;
}

void GuiTypingTextCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    renderBorder(RectI(offset, getExtent()), mProfile);

    const U32 textHeight = mProfile->mFont->getHeight();

    // Enumerate lines
    GFX->getDrawUtil()->setBitmapModulation(mProfile->mFontColor);
    for (dsize_t lineIteration = 0; lineIteration < mRenderedLines.size(); ++lineIteration)
    {
        const std::vector<TextBlockEntry>& currentRenderedLine = mRenderedLines[lineIteration];

        const Point2I lineOffset = offset + Point2I(0, textHeight * lineIteration);

        // Enumerate blocks in the line
        for (dsize_t blockIteration = 0; blockIteration < currentRenderedLine.size(); ++blockIteration)
        {
           const TextBlockEntry& currentRenderedBlock = currentRenderedLine[blockIteration];
           
           GFX->getDrawUtil()->drawText(mProfile->mFont, lineOffset, (char*)currentRenderedBlock.mText, mProfile->mFontColors);
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
       const std::vector<TextBlockEntry>& currentLine = mSourceLines[mLineIndex];

       if (mTextBlockIndex < currentLine.size())
       {
           const TextBlockEntry& currentBlock = currentLine[mTextBlockIndex];

           const dsize_t previousTextIndex = static_cast<dsize_t>(mTextBlockProgress);
           mTextBlockProgress += currentBlock.mTypingSpeed;
           const dsize_t nextTextIndex = static_cast<dsize_t>(mTextBlockProgress);

           // Copy into render buffer
           std::vector<TextBlockEntry>& renderedLine = mRenderedLines[mLineIndex];
           TextBlockEntry& renderedBlock = renderedLine[mTextBlockIndex];

           for (dsize_t iteration = previousTextIndex; iteration < currentBlock.mTextLength && iteration < nextTextIndex; ++iteration)
           {
              renderedBlock.mText[iteration] = currentBlock.mText[iteration];
           }

           if (nextTextIndex >= currentBlock.mTextLength)
           {
              ++mTextBlockIndex;
              mTextBlockProgress = 0.0f;
           }
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

void GuiTypingTextCtrl::parseTextInput(const char* text, const dsize_t textLength, std::vector<std::vector<TextBlockEntry>>& out)
{
   std::vector<TextBlockEntry> currentLine;

    // Enumerate all lines
    dsize_t currentLineStart = 0;
    for (dsize_t iteration = 0; iteration < textLength; ++iteration)
    {
        // If we encounter a newline character, copy the buffer into a new entry
        if (text[iteration] == '\n')
        {
            const char* copyTextPointer = text + currentLineStart;

            const dsize_t bytesCopied = iteration - currentLineStart;
            currentLine.push_back(TextBlockEntry(copyTextPointer, bytesCopied));

            out.push_back(currentLine);
            currentLine = std::vector<TextBlockEntry>();
            currentLineStart = iteration + 1; // Skip over the newline character
        }
    }

    // Copy remaining buffer into a new line
    currentLine = std::vector<TextBlockEntry>();
    if (currentLineStart < textLength)
    {
        const char* copyTextPointer = text + currentLineStart;
        currentLine.push_back(TextBlockEntry(copyTextPointer, textLength - currentLineStart));

        out.push_back(currentLine);
    }
}

