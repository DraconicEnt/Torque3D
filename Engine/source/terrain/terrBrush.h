#pragma once

#ifndef _EDITTSCTRL_H_
#include "gui/worldEditor/editTSCtrl.h"
#endif
#ifndef _TERRDATA_H_
#include "terrain/terrData.h"
#endif
#ifndef _UNDO_H_
#include "util/undo.h"
#endif

#include "terrain/terrSelection.h"
#include "terrain/terrDeformContext.h"

class TerrainEditor;

class Brush : public Selection
{
protected:

    TerrainEditor *   mTerrainEditor;
    Point2I           mSize;
    GridPoint         mGridPoint;
    Vector<S32>       mRenderList;

public:

    enum { MaxBrushDim = 256 };

    Brush(TerrainEditor * editor);
    virtual ~Brush(){};

    virtual const char *getType() const = 0;

    // Brush appears to intentionally bypass Selection's hash table, so we
    // override validate() here.
    bool validate() { return true; }
    void setPosition(const Point3F & pos);
    void setPosition(const Point2I & pos);
    const Point2I & getPosition();
    const GridPoint & getGridPoint();
    void setTerrain(TerrainBlock* terrain) { mGridPoint.terrainBlock = terrain; };
    Point2I getSize() const {return(mSize);}
    virtual void setSize(const Point2I & size){mSize = size;}

    void update();
    void render();

    virtual void rebuild() = 0;
    virtual void _renderOutline() = 0;
};

class BoxBrush : public Brush
{
public:

    BoxBrush(TerrainEditor * editor) : Brush(editor){}

    const char *getType() const { return "box"; }
    void rebuild();

protected:

    void _renderOutline();
};

class EllipseBrush : public Brush
{
public:

    EllipseBrush(TerrainEditor * editor) : Brush(editor){}

    const char *getType() const { return "ellipse"; }
    void rebuild();

protected:

    void _renderOutline();
};

class SelectionBrush : public Brush
{
public:

    SelectionBrush(TerrainEditor * editor);

    const char *getType() const { return "selection"; }
    void rebuild();
    void render(Vector<GFXVertexPCT> & vertexBuffer, S32 & verts, S32 & elems, S32 & prims, const LinearColorF & inColorFull, const LinearColorF & inColorNone, const LinearColorF & outColorFull, const LinearColorF & outColorNone) const;
    void setSize(const Point2I &){}

protected:

    void _renderOutline() {}
};