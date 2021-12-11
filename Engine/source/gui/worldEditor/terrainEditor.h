//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _TERRAINEDITOR_H_
#define _TERRAINEDITOR_H_

#ifndef _EDITTSCTRL_H_
#include "gui/worldEditor/editTSCtrl.h"
#endif
#ifndef _TERRDATA_H_
#include "terrain/terrData.h"
#endif
#ifndef _UNDO_H_
#include "util/undo.h"
#endif

#include "terrain/terrBrush.h"
#include "terrain/terrDeformContext.h"

class TerrainAction;

class TerrainEditor : public EditTSCtrl
{
	// XA: This methods where added to replace the friend consoleMethods.
	public:
      //! The deformation context used by this editor.
      TerrainDeformContext* mDeformContext;

      void attachTerrain(TerrainBlock *terrBlock);
      void detachTerrain(TerrainBlock *terrBlock);
		
      S32 getTerrainBlockCount() {return mTerrainBlocks.size();}
      TerrainBlock* getTerrainBlock(S32 index);
      void getTerrainBlocksMaterialList(Vector<StringTableEntry>& list);   // Returns consolidated list of all materials used on all terrain blocks

		void setBrushType(const char* type);
      const char* getBrushType() const;

		void setBrushSize(S32 w, S32 h);
		const char* getBrushPos();
		void setBrushPos(Point2I pos);
		
		void setAction(const char* action);
		const char* getActionName(U32 index);
		const char* getCurrentAction() const;
		S32 getNumActions();
		void processAction(const char* sAction);
			
		void resetSelWeights(bool clear);
		void clearSelection();
		
		S32 getNumTextures();

		void markEmptySquares();
		
		void mirrorTerrain(S32 mirrorIndex);
		
      TerrainBlock* getActiveTerrain() { return mActiveTerrain; };

      void setGridUpdateMinMax() 
      {
         mDeformContext->mGridUpdateMax.set( S32_MAX, S32_MAX );
         mDeformContext->mGridUpdateMin.set( 0, 0 );
      }
      
      void submitMaterialUndo( String actionName );
      void onMaterialUndo( TerrainBlock *terr );

      void autoMaterialLayer( F32 mMinHeight, F32 mMaxHeight, F32 mMinSlope, F32 mMaxSlope, F32 mCoverage );

	private:	

      typedef EditTSCtrl Parent;

      TerrainBlock* mActiveTerrain;

      // A list of all of the TerrainBlocks this editor can edit
      VectorPtr<TerrainBlock*> mTerrainBlocks;

      U32 mMouseDownSeq;

      bool                       mMouseDown;
      PlaneF                     mMousePlane;
      Point3F                    mMousePos;
      Brush *                    mMouseBrush;
      bool                       mBrushChanged;
      bool                       mRenderBrush;
      F32                        mBrushPressure;
      Point2I                    mBrushSize;
      F32                        mBrushSoftness;
      Vector<TerrainAction *>    mActions;
      TerrainAction *            mCurrentAction;
      bool                       mInAction;
      Selection                  mDefaultSel;
      bool                       mSelectionLocked;      
      
      S32                        mPaintIndex;

      Selection *                mCurrentSel;

      class TerrainEditorUndoAction : public UndoAction
      {
      public:

         TerrainEditorUndoAction( const UTF8* actionName ) 
            :  UndoAction( actionName ),
               mTerrainEditor( NULL ),
               mSel( NULL )
         {
         }

         virtual ~TerrainEditorUndoAction()
         {
            delete mSel;
         }

         TerrainEditor *mTerrainEditor;

         Selection *mSel;
         
         virtual void undo();
         virtual void redo() { undo(); }
      };

      void submitUndo( Selection *sel );

      class TerrainMaterialUndoAction : public UndoAction
      {
      public:

         TerrainMaterialUndoAction( const UTF8 *actionName )
            : UndoAction( actionName ),
              mEditor( NULL ),
              mTerrain( NULL )
         {
         }

         TerrainEditor *mEditor;
         TerrainBlock *mTerrain;
         Vector<U8> mLayerMap;         
         Vector<TerrainMaterial*> mMaterials;

         virtual void undo();
         virtual void redo();
      };

      bool mIsDirty; // dirty flag for writing terrain.
      bool mIsMissionDirty; // dirty flag for writing mission.

      GFXStateBlockRef mStateBlock;

   public:

      TerrainEditor();
      ~TerrainEditor();

      // conversion functions
      // Returns true if the grid position is on the main tile
      bool isMainTile(const GridPoint & gPoint) const;

      // Takes a world point and find the "highest" terrain underneath it
      // Returns true if the returned GridPoint includes a valid terrain and grid position
      TerrainBlock* getTerrainUnderWorldPoint(const Point3F & wPos);

      // Converts a GridPoint to a world position
      bool gridToWorld(const GridPoint & gPoint, Point3F & wPos);
      bool gridToWorld(const Point2I & gPos, Point3F & wPos, TerrainBlock* terrain);

      // Converts a world position to a grid point
      // If the grid point does not have a TerrainBlock already it will find the nearest
      // terrian under the world position
      bool worldToGrid(const Point3F & wPos, GridPoint & gPoint);
      bool worldToGrid(const Point3F & wPos, Point2I & gPos, TerrainBlock* terrain = NULL);

      //bool getGridInfo(const Point3F & wPos, GridInfo & info);
      // Gets the grid info for a point on a TerrainBlock's grid
      bool getGridInfo(const GridPoint & gPoint, GridInfo & info);
      bool getGridInfo(const Point2I & gPos, GridInfo & info, TerrainBlock* terrain);

      // Returns a list of infos for all points on the terrain that are at that point in space
      void getGridInfos(const GridPoint & gPoint, Vector<GridInfo>& infos);

      void setGridInfo(const GridInfo & info, bool checkActive = false);
      void setGridInfoHeight(const GridInfo & info);
      void gridUpdateComplete( bool materialChanged = false );
      void materialUpdateComplete();
      void processActionTick(U32 sequence);

      TerrainBlock* collide(const Gui3DMouseEvent & event, Point3F & pos);
      void lockSelection(bool lock) { mSelectionLocked = lock; };

      Selection * getCurrentSel(){return(mCurrentSel);}
      void setCurrentSel(Selection * sel) { mCurrentSel = sel; }
      void resetCurrentSel() {mCurrentSel = &mDefaultSel; }

      S32 getPaintMaterialIndex() const { return mPaintIndex; }

      void setBrushPressure( F32 pressure );
      F32 getBrushPressure() const { return mBrushPressure; }
      
      void setBrushSoftness( F32 softness );
      F32 getBrushSoftness() const { return mBrushSoftness; }

      Point2I getBrushSize() { return(mBrushSize); }
      
      TerrainBlock* getTerrainBlock() const { return mActiveTerrain; }
      TerrainBlock* getClientTerrain( TerrainBlock *serverTerrain = NULL ) const;
      bool terrainBlockValid() { return(mActiveTerrain ? true : false); }
      void setDirty() { mIsDirty = true; }
      void setMissionDirty()  { mIsMissionDirty = true; }

      TerrainAction * lookupAction(const char * name);

   private:


      // terrain interface functions
      // Sets a height at a grid point
      void setGridHeight(const GridPoint & gPoint, const F32 height);

      ///
      U8 getGridMaterial( const GridPoint &gPoint ) const;

      ///
      void setGridMaterial( const GridPoint & gPoint, U8 index );

      // Gets the material group of a specific spot on a TerrainBlock's grid
      U8 getGridMaterialGroup(const GridPoint & gPoint);

      // Sets a material group for a spot on a TerrainBlock's grid
      void setGridMaterialGroup(const GridPoint & gPoint, U8 group);

      //
      void updateBrush(Brush & brush, const Point2I & gPos);      

      //
      void renderSelection(const Selection & sel, const LinearColorF & inColorFull, const LinearColorF & inColorNone, const LinearColorF & outColorFull, const LinearColorF & outColorNone, bool renderFill, bool renderFrame);
      void renderBrush(const Brush & brush, const LinearColorF & inColorFull, const LinearColorF & inColorNone, const LinearColorF & outColorFull, const LinearColorF & outColorNone, bool renderFill, bool renderFrame);
      void renderBorder();

   public:
      
      // persist field data - these are dynamic
      bool                 mRenderBorder;
      F32                  mBorderHeight;
      ColorI               mBorderFillColor;
      ColorI               mBorderFrameColor;
      bool                 mBorderLineMode;
      bool                 mSelectionHidden;
      bool                 mRenderVertexSelection;
      bool                 mRenderSolidBrush;
      bool                 mProcessUsesBrush;

      //
      F32                  mAdjustHeightVal;
      F32                  mSetHeightVal;
      F32                  mScaleVal;
      F32                  mSmoothFactor;
      F32                  mNoiseFactor;
      S32                  mMaterialGroup;
      F32                  mSoftSelectRadius;
      StringTableEntry     mSoftSelectFilter;
      StringTableEntry     mSoftSelectDefaultFilter;
      F32                  mAdjustHeightMouseScale;
      Point2I              mMaxBrushSize;

      F32 mSlopeMinAngle;
      F32 mSlopeMaxAngle;

   public:

      // SimObject
      bool onAdd();
      void onDeleteNotify(SimObject * object);

      static void initPersistFields();

      // GuiControl
      bool onWake();
      void onSleep();      

      // EditTSCtrl
      bool onInputEvent( const InputEventInfo & evt );
      void on3DMouseUp( const Gui3DMouseEvent & evt );
      void on3DMouseDown( const Gui3DMouseEvent & evt );
      void on3DMouseMove( const Gui3DMouseEvent & evt );
      void on3DMouseDragged( const Gui3DMouseEvent & evt );
      bool onMouseWheelUp( const GuiEvent & evt );
      bool onMouseWheelDown( const GuiEvent & evt );            
      void get3DCursor( GuiCursor *&cursor, bool &visible, const Gui3DMouseEvent &evt );   
      void onPreRender();
      void renderScene(const RectI & updateRect);
      void renderGui( Point2I offset, const RectI &updateRect );
      void updateGuiInfo();

      /// Reorder material at the given index to the new position, changing the order in which it is rendered / blended
      void reorderMaterial( S32 index, S32 orderPos );

      //
      Point3F getMousePos(){return(mMousePos);};

      void renderPoints( const Vector<GFXVertexPCT> &pointList );


      DECLARE_CONOBJECT(TerrainEditor);     
};

inline void TerrainEditor::setGridInfoHeight(const GridInfo & info)
{
   setGridHeight(info.mGridPoint, info.mHeight);
}

#endif
