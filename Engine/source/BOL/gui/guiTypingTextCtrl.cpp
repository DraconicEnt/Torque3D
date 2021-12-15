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
   "@brief Gui control that 'types' out a set of text, primarily for use in eg. character dialog.\n\n"

   "@tsexample\n"
   "  new GuiTypingTextCtrl()\n"
   "  {\n"
   "     text = \"Hello World\";\n"
   "     textID = \"\"STR_HELLO\"\";\n"
   "     typingSpeed = 0.005; // Characters per 32ms\n"
   "      //Properties not specific to this control have been omitted from this example.\n"
   "  };\n"
   "@endtsexample\n\n"

   "@see GuiControl\n"
   "@see Localization\n\n"
   "@ingroup GuiCore\n"
);

IMPLEMENT_CALLBACK( GuiTypingTextCtrl, onLineTyped, void, (U32 lineNumber), ( lineNumber ),
                    "Called when a line is fully typed out." );
IMPLEMENT_CALLBACK( GuiTypingTextCtrl, onCharactersTyped, void, (U32 lineNumber, U32 startCharacterNumber, U32 endCharacterNumber ), ( lineNumber, startCharacterNumber, endCharacterNumber ),
                    "Called when a set of characters is typed out. May represent multiple characters being typed depending on typing speed." );
IMPLEMENT_CALLBACK( GuiTypingTextCtrl, onTextTyped, void, (), (),
                    "Called when the entirety of the text is fully typed out." );
IMPLEMENT_CALLBACK( GuiTypingTextCtrl, onScrollBegin, void, (U32 lineNumber), (lineNumber),
                    "Called when the control begins automatically scrolling." );

GuiTypingTextCtrl::GuiTypingTextCtrl() : GuiControl()
{
    mSourceText = StringTable->EmptyString();
    mLineIndex = 0;
    mAutoResize = false;
    mScrollSpeed = 0.0f;
    mCurrentScroll = 0.0f;
    mTextBlockIndex = 0;
    mScrolling = false;
    mScrollTrigger = 0.9f;
    mTextBlockProgress = 0.0f;
}

void GuiTypingTextCtrl::initPersistFields()
{
    addProtectedField("text", TypeCaseString, Offset(mSourceText, GuiTypingTextCtrl), setText, getTextProperty,
                      "The text to show on the control.");
    addField("autoResize", TypeBool, Offset(mAutoResize, GuiTypingTextCtrl), "Whether or not the control should auto resize.");
    addField("scrollSpeed", TypeF32, Offset(mScrollSpeed, GuiTypingTextCtrl), "The vertical scroll speed when the text is typing out.");
    addField("scrollTrigger", TypeF32, Offset(mScrollTrigger, GuiTypingTextCtrl), "The scroll trigger - the percentage of the vertical size at which scrolling should begin.");
    addField("scrolling", TypeBool, Offset(mScrolling, GuiTypingTextCtrl), "Whether or not the text should be scrolling.");
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
    mScrolling = false;
    mCurrentScroll = 0.0f;
    mTextBlockProgress = 0.0f;
}

void GuiTypingTextCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    Parent::onRender(offset, updateRect);

    renderBorder(RectI(offset, getExtent()), mProfile);

    const U32 textHeight = mProfile->mFont->getHeight();
    U32 maxWidth = 0;

    // Enumerate lines
    GFX->getDrawUtil()->setBitmapModulation(mProfile->mFontColor);
    for (dsize_t lineIteration = 0; lineIteration < mRenderedLines.size(); ++lineIteration)
    {
        const std::vector<TextBlockEntry>& currentRenderedLine = mRenderedLines[lineIteration];

        const Point2I lineOffset = offset + Point2I(0, (textHeight * lineIteration) - mCurrentScroll);

        // Enumerate blocks in the line
        for (dsize_t blockIteration = 0; blockIteration < currentRenderedLine.size(); ++blockIteration)
        {
           const TextBlockEntry& currentRenderedBlock = currentRenderedLine[blockIteration];

           const U32 textWidth = mProfile->mFont->getStrWidth(currentRenderedBlock.mText);
           maxWidth = textWidth > maxWidth ? textWidth : maxWidth;

           GFX->getDrawUtil()->drawText(mProfile->mFont, lineOffset, (char*)currentRenderedBlock.mText, mProfile->mFontColors);
        }
    }

    // Set new extents
    if (mAutoResize)
    {
        const Point2I newExtent = Point2I(maxWidth, textHeight * mRenderedLines.size());
        setExtent(newExtent);
    }

    // render the child controls
    renderChildControls(offset, updateRect);
}

void GuiTypingTextCtrl::interpolateTick( F32 delta )
{

}

void GuiTypingTextCtrl::processTick()
{
   const U32 textHeight = mProfile->mFont->getHeight();

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

           if (nextTextIndex != previousTextIndex)
           {
               const F32 percentageOccupied = static_cast<F32>(textHeight * mLineIndex) / static_cast<F32>(getExtent().y);
               if (percentageOccupied >= mScrollTrigger)
               {
                   mScrolling = true;
                   onScrollBegin_callback(mLineIndex);
               }

               onCharactersTyped_callback(mLineIndex, previousTextIndex, nextTextIndex);
           }

           if (nextTextIndex >= currentBlock.mTextLength)
           {
              ++mTextBlockIndex;
              mTextBlockProgress = 0.0f;
           }
       }
       else
       {
           onLineTyped_callback(mLineIndex);

           ++mLineIndex;
           mTextBlockIndex = 0;
           mTextBlockProgress = 0.0f;

           if (mLineIndex >= mSourceLines.size())
           {
               onTextTyped_callback();
           }
       }
   }

   if (mScrolling)
   {
       mCurrentScroll += mScrollSpeed;
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

