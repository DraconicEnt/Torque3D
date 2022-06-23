#include "gfxDrawFunctions.h"

static GFXStateBlock* sBitmapStretchSB = NULL;
static GFXStateBlock* sRectFillSB;

static void initStateBlocks()
{
    if (!sBitmapStretchSB)
    {
        // No Engine Mods: For now we just create the state blocks here...
        GFXStateBlockDesc bitmapStretchSR;
        bitmapStretchSR.setCullMode(GFXCullNone);
        bitmapStretchSR.setZReadWrite(false);
        bitmapStretchSR.setBlend(true, GFXBlendSrcAlpha, GFXBlendInvSrcAlpha);
        bitmapStretchSR.samplersDefined = true;
        bitmapStretchSR.setColorWrites(true, true, true, false); // NOTE: comment this out if alpha write is needed

        sBitmapStretchSB = GFX->createStateBlock(bitmapStretchSR);
    }

    if (!sRectFillSB)
    {
        GFXStateBlockDesc rectFill;
        rectFill.setCullMode(GFXCullNone);
        rectFill.setZReadWrite(false);
        rectFill.setBlend(true, GFXBlendSrcAlpha, GFXBlendInvSrcAlpha);
        sRectFillSB = GFX->createStateBlock(rectFill);
    }
}

void GFXDrawFunctions_drawPolygonTexture( const GFXStateBlockDesc& desc, const Point3F* points, U32 numPoints, const ColorI* pointColors, UVMode uvMode, const ColorI& color, const MatrixF* xfm /* = NULL */, GFXTexHandle texture)
{
   const bool isWireframe = ( desc.fillMode == GFXFillWireframe );
   const U32 numVerts = isWireframe ? numPoints + 1 : numPoints;
   GFXVertexBufferHandle< GFXVertexPCT > verts( GFX, numVerts, GFXBufferTypeVolatile );
   verts.lock();

   // Calculate bounds of the shape
   Point2F upperLeft(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
   Point2F lowerRight(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
   for( U32 i = 0; i < numPoints; ++i )
   {
       if (points[i].x <= upperLeft.x)
       {
           upperLeft.x = points[i].x;
       }

       if (points[i].y <= upperLeft.y)
       {
           upperLeft.y = points[i].y;
       }

       if (points[i].x >= lowerRight.x)
       {
           lowerRight.x = points[i].x;
       }

       if (points[i].y >= lowerRight.y)
       {
           lowerRight.y = points[i].y;
       }
   }

   const Point2F shapeDimensions = Point2F(lowerRight.x - upperLeft.x, lowerRight.y - upperLeft.y);

   Point2F radialMapUVs[3] =
   {
       Point2F(0.0f, -1.0f),
       Point2F(-1.0f, 1.0f),
       Point2F(1.0f, 1.0f)
   };

   for( U32 i = 0; i < numPoints; ++ i )
   {
      verts[ i ].point = points[ i ];

      if (pointColors)
      {
          verts[ i ].color = pointColors[i];
      }
      else
      {
          verts[ i ].color = color;
      }

      switch (uvMode)
      {
          case UVMode::TextureMap:
              verts[ i ].texCoord = Point2F(points[i].x / shapeDimensions.x, points[i].y / shapeDimensions.y);
              break;
          case UVMode::RadialMap:
              verts[ i ].texCoord = radialMapUVs[i % 3];
              break;
      }
   }

   if( xfm )
   {
      for( U32 i = 0; i < numPoints; ++ i )
         xfm->mulP( verts[ i ].point );
   }
   
   if( isWireframe )
   {
      verts[ numVerts - 1 ].point = verts[ 0 ].point;
      verts[ numVerts - 1 ].color = color;
   }

   verts.unlock();

   GFX->setStateBlockByDesc( desc );

   GFX->setVertexBuffer( verts );

   if (texture)
   {
       GFX->setTexture(0, texture);
       GFX->setupGenericShaders(GFXDevice::GSModColorTexture);
   }
   else
   {
       GFX->setupGenericShaders();
   }

   if( desc.fillMode == GFXFillWireframe )
       GFX->drawPrimitive( GFXLineStrip, 0, numPoints );
   else
       GFX->drawPrimitive( GFXTriangleStrip, 0, numPoints - 2 );
}

void GFXDrawFunctions_drawLineWidth( F32 x1, F32 y1, F32 z1, F32 x2, F32 y2, F32 z2, const ColorI& color, F32 width)
{
    GFXDrawFunctions_drawLineWidthTextured(x1, y1, z1, x2, y2, z2, color, width, NULL);
}

void GFXDrawFunctions_drawLineWidthTextured( F32 x1, F32 y1, F32 z1, F32 x2, F32 y2, F32 z2, const ColorI& color, F32 width, GFXTextureObject* texture)
{
    GFXVertexBufferHandle<GFXVertexPCT> verts(GFX, 4, GFXBufferTypeVolatile );
    verts.lock();

    // We generate 4 vertices to draw a box, to do this we take the angle between the two points
    // and use that to determine the alignment of the box
    F32 angle = mAtan2(y1 - y2, x1 - x2);
    F32 angleLeft = angle - Float_HalfPi;
    F32 angleRight = angle + Float_HalfPi;

    F32 leftX = cos(angleLeft) * (width / 2);
    F32 leftY = sin(angleLeft) * (width / 2);
    F32 rightX = cos(angleRight) * (width / 2);
    F32 rightY = sin(angleRight) * (width / 2);

    // Once we have the angles calculated, use this information to generate a line
    verts[0].point.set( x1 + leftX, y1 + leftY, z1 );
    verts[1].point.set( x1 + rightX, y1 + rightY, z2 );
    verts[2].point.set( x2 + leftX, y2 + leftY, z1 );
    verts[3].point.set( x2 + rightX, y2 + rightY, z2 );

    verts[0].color = color;
    verts[1].color = color;
    verts[2].color = color;
    verts[3].color = color;

    // FIXME: Calculate
    if (texture)
    {
        verts[0].texCoord.set( -1.0f,  -1.0f );
        verts[1].texCoord.set( 1.0f, -1.0f );
        verts[2].texCoord.set( -1.0f,  1.0f );
        verts[3].texCoord.set( 1.0f, 1.0f );
    }

    verts.unlock();

    GFX->setVertexBuffer( verts );

    initStateBlocks();
    if (texture)
    {
        GFX->setStateBlock(sBitmapStretchSB);
        GFX->setTexture(0, texture);
        GFX->setupGenericShaders(GFXDevice::GSModColorTexture);
    }
    else
    {
        GFX->setStateBlock(sRectFillSB);
        GFX->setupGenericShaders();
    }

    GFX->drawPrimitive( GFXTriangleStrip, 0, 2 );
}