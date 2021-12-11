#include "terrain/terrBrush.h"
#include "terrain/terrSelection.h"
#include "gui/worldEditor/terrainEditor.h"

Brush::Brush(TerrainEditor * editor) :
        mTerrainEditor(editor)
{
    mSize = mTerrainEditor->getBrushSize();
}

const Point2I & Brush::getPosition()
{
    return(mGridPoint.gridPos);
}

const GridPoint & Brush::getGridPoint()
{
    return mGridPoint;
}

void Brush::setPosition(const Point3F & pos)
{
    PROFILE_SCOPE( TerrainEditor_Brush_SetPosition_Point3F );

    mTerrainEditor->worldToGrid(pos, mGridPoint);
    update();
}

void Brush::setPosition(const Point2I & pos)
{
    PROFILE_SCOPE( TerrainEditor_Brush_SetPosition_Point2I );

    mGridPoint.gridPos = pos;
    update();
}

void Brush::update()
{
    PROFILE_SCOPE( TerrainEditor_Brush_update );

    if ( mGridPoint.terrainBlock )
        rebuild();
}

void Brush::render()
{
    PROFILE_SCOPE( TerrainEditor_Brush_Render );

    // Render the brush's outline via the derived brush class.
    _renderOutline();

    // Render the brush's interior grid points.

    const U32 pointCount = mSize.x * mSize.y;
    if ( pointCount == 0 )
        return;

    if ( mRenderList.empty() || empty() )
        return;

    Vector<GFXVertexPCT> pointList;
    pointList.reserve( pointCount );

    for(S32 x = 0; x < mSize.x; x++)
    {
        for(S32 y = 0; y < mSize.y; y++)
        {
            S32 id = mRenderList[x*mSize.x+y];
            if ( id == -1 )
                continue;

            const GridInfo &gInfo = (*this)[ id ];

            Point3F pos;
            mTerrainEditor->gridToWorld( gInfo.mGridPoint.gridPos, pos, gInfo.mGridPoint.terrainBlock );

            if ( !mTerrainEditor->project( pos, &pos ) )
                continue;

            pointList.increment();
            GFXVertexPCT &pointInfo = pointList.last();

            pointInfo.point = pos;

            pointInfo.color.set( 255, 0, 255, gInfo.mWeight * 255 );

            pointInfo.texCoord.set( 1.0f, 0.0f );
        }
    }

    mTerrainEditor->renderPoints( pointList );
}

void BoxBrush::rebuild()
{
    PROFILE_SCOPE( TerrainEditor_BoxBrush_Rebuild );

    reset();

    const F32 squareSize = mGridPoint.terrainBlock->getSquareSize();

    mRenderList.setSize(mSize.x*mSize.y);

    Point3F center( F32(mSize.x - 1) / 2.0f * squareSize, F32(mSize.y - 1) / 2.0f * squareSize, 0.0f );

    Filter filter;
    filter.set(1, &mTerrainEditor->mSoftSelectFilter);

    const Point3F mousePos = mTerrainEditor->getMousePos();

    F32 xFactorScale = center.x / ( center.x + 0.5f );
    F32 yFactorScale = center.y / ( center.y + 0.5f );

    const F32 softness = mTerrainEditor->getBrushSoftness();
    const F32 pressure = mTerrainEditor->getBrushPressure();

    Point3F posw( 0,0,0 );
    Point2I posg( 0,0 );
    Vector<GridInfo> infos;

    for ( S32 x = 0; x < mSize.x; x++ )
    {
        for(S32 y = 0; y < mSize.y; y++)
        {
            F32 xFactor = 0.0f;
            if ( center.x > 0 )
                xFactor = mAbs( center.x - x ) / center.x * xFactorScale;

            F32 yFactor = 0.0f;
            if ( center.y > 0 )
                yFactor = mAbs( center.y - y ) / center.y * yFactorScale;

            S32 &rl = mRenderList[x*mSize.x+y];

            posw.x = mousePos.x + (F32)x * squareSize - center.x;
            posw.y = mousePos.y + (F32)y * squareSize - center.y;
            // round to grid coords
            GridPoint gridPoint = mGridPoint;
            mTerrainEditor->worldToGrid( posw, gridPoint );

            // Check that the grid point is valid within the terrain.  This assumes
            // that there is no wrap around past the edge of the terrain.
            if(!gridPoint.terrainBlock->isPointInTerrain(gridPoint.gridPos))
            {
                rl = -1;
                continue;
            }

            infos.clear();
            mTerrainEditor->getGridInfos( gridPoint, infos );

            for (U32 z = 0; z < infos.size(); z++)
            {
                infos[z].mWeight = pressure *
                                   mLerp( infos[z].mWeight, filter.getValue(xFactor > yFactor ? xFactor : yFactor), softness );

                push_back(infos[z]);
            }

            rl = size()-1;
        }
    }
}

void BoxBrush::_renderOutline()
{
    F32 squareSize = mGridPoint.terrainBlock->getSquareSize();

    RayInfo ri;
    Point3F start( 0, 0, 5000.0f );
    Point3F end( 0, 0, -5000.0f );
    bool hit;

    Vector<Point3F> pointList;
    pointList.reserve( 64 );

    const ColorI col( 255, 0, 255, 255 );

    const Point3F &mousePos = mTerrainEditor->getMousePos();

    static const Point2F offsetArray [5] =
            {
                    Point2F( -1, -1 ),
                    Point2F( 1, -1 ),
                    Point2F( 1, 1 ),
                    Point2F( -1, 1 ),
                    Point2F( -1, -1 ) // repeat of offset[0]
            };

    // 64 total steps, 4 sides to the box, 16 steps per side.
    // 64 / 4 = 16
    const U32 steps = 16;

    for ( S32 i = 0; i < 4; i++ )
    {
        const Point2F &offset = offsetArray[i];
        const Point2F &next = offsetArray[i+1];

        for ( S32 j = 0; j < steps; j++ )
        {
            F32 frac = (F32)j / ( (F32)steps - 1.0f );

            Point2F tmp;
            tmp.interpolate( offset, next, frac );

            start.x = end.x = mousePos.x + tmp.x * squareSize * 0.5f * (F32)mSize.x;
            start.y = end.y = mousePos.y + tmp.y * squareSize * 0.5f * (F32)mSize.y;

            hit = gServerContainer.castRay( start, end, TerrainObjectType, &ri );

            if ( hit )
                pointList.push_back( ri.point );
        }
    }

    mTerrainEditor->drawLineList( pointList, col, 1.0f );
}

void EllipseBrush::rebuild()
{
    PROFILE_SCOPE( TerrainEditor_EllipseBrush_Rebuild );

    reset();

    const F32 squareSize = mGridPoint.terrainBlock->getSquareSize();

    mRenderList.setSize(mSize.x*mSize.y);

    Point3F center( F32(mSize.x - 1) / 2.0f * squareSize, F32(mSize.y - 1) / 2.0f * squareSize, 0.0f );

    Filter filter;
    filter.set(1, &mTerrainEditor->mSoftSelectFilter);

    const Point3F mousePos = mTerrainEditor->getMousePos();

    // a point is in a circle if:
    // x^2 + y^2 <= r^2
    // a point is in an ellipse if:
    // (ax)^2 + (by)^2 <= 1
    // where a = 1/halfEllipseWidth and b = 1/halfEllipseHeight

    // for a soft-selected ellipse,
    // the factor is simply the filtered: ((ax)^2 + (by)^2)

    F32 a = 1.0f / (F32(mSize.x) * squareSize * 0.5f);
    F32 b = 1.0f / (F32(mSize.y) * squareSize * 0.5f);

    const F32 softness = mTerrainEditor->getBrushSoftness();
    const F32 pressure = mTerrainEditor->getBrushPressure();

    Point3F posw( 0,0,0 );
    Point2I posg( 0,0 );
    Vector<GridInfo> infos;

    for ( S32 x = 0; x < mSize.x; x++ )
    {
        for ( S32 y = 0; y < mSize.y; y++ )
        {
            F32 xp = center.x - x * squareSize;
            F32 yp = center.y - y * squareSize;

            F32 factor = (a * a * xp * xp) + (b * b * yp * yp);
            if ( factor > 1 )
            {
                mRenderList[x*mSize.x+y] = -1;
                continue;
            }

            S32 &rl = mRenderList[x*mSize.x+y];

            posw.x = mousePos.x + (F32)x * squareSize - center.x;
            posw.y = mousePos.y + (F32)y * squareSize - center.y;

            // round to grid coords
            GridPoint gridPoint = mGridPoint;
            mTerrainEditor->worldToGrid( posw, gridPoint );

            // Check that the grid point is valid within the terrain.  This assumes
            // that there is no wrap around past the edge of the terrain.
            if ( !gridPoint.terrainBlock->isPointInTerrain( gridPoint.gridPos ) )
            {
                rl = -1;
                continue;
            }

            infos.clear();
            mTerrainEditor->getGridInfos( gridPoint, infos );

            for ( U32 z = 0; z < infos.size(); z++ )
            {
                infos[z].mWeight = pressure * mLerp( infos[z].mWeight, filter.getValue( factor ), softness );
                push_back(infos[z]);
            }

            rl = size()-1;
        }
    }
}

void EllipseBrush::_renderOutline()
{
    F32 squareSize = mGridPoint.terrainBlock->getSquareSize();

    RayInfo ri;
    Point3F start( 0, 0, 5000.0f );
    Point3F end( 0, 0, -5000.0f );
    bool hit;

    Vector<Point3F> pointList;

    ColorI col( 255, 0, 255, 255 );

    const U32 steps = 64;

    const Point3F &mousePos = mTerrainEditor->getMousePos();

    for ( S32 i = 0; i < steps; i++ )
    {
        F32 radians = (F32)i / (F32)(steps-1) * M_2PI_F;
        VectorF vec(0,1,0);
        MathUtils::vectorRotateZAxis( vec, radians );

        start.x = end.x = mousePos.x + vec.x * squareSize * (F32)mSize.x * 0.5f;
        start.y = end.y = mousePos.y + vec.y * squareSize * (F32)mSize.y * 0.5f;

        hit = gServerContainer.castRay( start, end, TerrainObjectType, &ri );

        if ( hit )
            pointList.push_back( ri.point );
    }

    mTerrainEditor->drawLineList( pointList, col, 1.0f );
}

SelectionBrush::SelectionBrush(TerrainEditor * editor) :
        Brush(editor)
{
    //... grab the current selection
}

void SelectionBrush::rebuild()
{
    reset();
    //... move the selection
}

void SelectionBrush::render(Vector<GFXVertexPCT> & vertexBuffer, S32 & verts, S32 & elems, S32 & prims, const LinearColorF & inColorFull, const LinearColorF & inColorNone, const LinearColorF & outColorFull, const LinearColorF & outColorNone) const
{
    //... render the selection
}
