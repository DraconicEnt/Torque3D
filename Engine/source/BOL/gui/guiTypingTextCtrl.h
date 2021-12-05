#pragma once

#include <vector>

#ifndef _GFONT_H_
#include "gfx/gFont.h"
#endif
#ifndef _GUITYPES_H_
#include "gui/core/guiTypes.h"
#endif
#ifndef _GUITEXTCTRL_H_
#include "gui/core/guiControl.h"
#endif
#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif

class GuiTypingTextCtrl : public GuiControl, public ITickable
{
private:
   typedef GuiControl Parent;

protected:
    /**
     * @brief A struct representing a block of text to be drawn in the element.
     */
    struct TextBlockEntry
    {
        char* mText;
        dsize_t mTextLength;
        F32 mTypingSpeed;

        /**
         * @brief Constructor for copying source text data.
         * @param source The source text to copy.
         * @param sourceLength The length of the source text to copy.
         */
        TextBlockEntry(const char* source, const dsize_t sourceLength)
        {
            mTypingSpeed = 0.05f;
            mTextLength = sourceLength;
            mText = (char*)dMalloc(sizeof(char) * sourceLength);
            dMemcpy(mText, source, sourceLength);
        }

        TextBlockEntry(const TextBlockEntry& copied)
        {
           mTextLength = copied.mTextLength;
           mTypingSpeed = copied.mTypingSpeed;
           mText = (char*)dMalloc(sizeof(char) * copied.mTextLength);
           dMemcpy(mText, copied.mText, mTextLength);
        }

        /**
         * @brief Constructor for building a blank text block, used for
         * assembling the render buffer.
         * @param bufferSize The size of the buffer.
         */
        TextBlockEntry(const dsize_t bufferSize)
        {
            mTextLength = bufferSize;
            mTypingSpeed = 0.05f;
            mText = (char*)dMalloc(sizeof(char) * bufferSize);
            dMemset(mText, 0x00, sizeof(char) * bufferSize);
        }

        TextBlockEntry()
        {
            mTextLength = 0;
            mText = NULL;
            mTypingSpeed = 0.05f;
        }

        ~TextBlockEntry()
        {
            if (mText)
            {
                dFree(mText);
                mText = NULL;
            }
        }
    };

    /// @name Callbacks
    /// @{

    DECLARE_CALLBACK( void, onLineTyped, (U32 lineNumber) );
    DECLARE_CALLBACK( void, onCharactersTyped, (U32 lineNumber, U32 characterNumber) );
    DECLARE_CALLBACK( void, onTextTyped, () );

    /// @}

    //! The source text provided to the control. Primarily used for reading back the text.
    StringTableEntry mSourceText;

    //! Current text block progress.
    F32 mTextBlockProgress;

    //! Current text block index.
    dsize_t mTextBlockIndex;

    //! Current line index.
    dsize_t mLineIndex;

    //! A vector of lines made up of text block entries. This is the source of the drawn data.
    std::vector<std::vector<TextBlockEntry>> mSourceLines;

    //! A vector of lines made up of text block entries. This is the rendered data per frame.
    std::vector<std::vector<TextBlockEntry>> mRenderedLines;

    //! Whether or not the control should auto resize as text is inserted into it.
    bool mAutoResize;

    void parseTextInput(const char* text, const dsize_t textLength, std::vector<std::vector<TextBlockEntry>>& out);

public:

   //creation methods
   DECLARE_CONOBJECT(GuiTypingTextCtrl);
   DECLARE_CATEGORY( "Gui Text" );
   DECLARE_DESCRIPTION( "A control that displays a single line of text." );

   GuiTypingTextCtrl();
   static void initPersistFields();

   virtual void setText(const char *newText);
   const char* getText() { return mSourceText; }

   virtual void interpolateTick( F32 delta );

   virtual void processTick();

   virtual void advanceTime( F32 timeDelta );

   //rendering methods
   void onRender(Point2I offset, const RectI &updateRect);

   // Text Property Accessors
   static bool setText(void *object, const char *index, const char *data)
   {
       static_cast<GuiTypingTextCtrl*>(object)->setText(data);
       return true;
   }

   static const char* getTextProperty(void* obj, const char* data)
   {
       return static_cast<GuiTypingTextCtrl*>(obj)->getText();
   }
};
