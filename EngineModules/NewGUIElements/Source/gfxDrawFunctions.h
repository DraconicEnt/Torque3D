#pragma once

#include "platform/platform.h"
#include "gfx/gfxDrawUtil.h"

#include "core/frameAllocator.h"
#include "core/strings/stringFunctions.h"
#include "core/strings/unicode.h"
#include "math/util/frustum.h"
#include "math/util/sphereMesh.h"
#include "math/mathUtils.h"
#include "gfx/gfxFontRenderBatcher.h"
#include "gfx/gfxTransformSaver.h"
#include "gfx/gfxPrimitiveBuffer.h"
#include "gfx/primBuilder.h"
#include "gfx/gfxDebugEvent.h"

#include "math/mPolyhedron.impl.h"

enum UVMode
{
    TextureMap,
    RadialMap
};

void GFXDrawFunctions_drawPolygonTexture( const GFXStateBlockDesc& desc, const Point3F* points, U32 numPoints, const ColorI* pointColors, UVMode uvMode, const ColorI& color, const MatrixF* xfm /* = NULL */, GFXTexHandle texture);
void GFXDrawFunctions_drawLineWidth( F32 x1, F32 y1, F32 z1, F32 x2, F32 y2, F32 z2, const ColorI& color, F32 width);
void GFXDrawFunctions_drawLineWidthTextured( F32 x1, F32 y1, F32 z1, F32 x2, F32 y2, F32 z2, const ColorI& color, F32 width, GFXTextureObject* texture);