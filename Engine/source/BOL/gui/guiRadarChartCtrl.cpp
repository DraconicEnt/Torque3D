#include "platform/platform.h"
#include "BOL/gui/guiRadarChartCtrl.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "T3D/gameBase/gameConnection.h"

IMPLEMENT_CONOBJECT(GuiRadarChartCtrl);

ConsoleDocClass( GuiRadarChartCtrl,
                 "@brief A gui control used to indicate relative strengths on a polygon.\n\n"
);

GuiRadarChartCtrl::GuiRadarChartCtrl() : GuiControl()
{
    mNumberOfSides = 4;
    mRotation = 0.0f;
    mOuterWidth = 10.0f;
    mOuterColor = ColorI::RED;
}

void GuiRadarChartCtrl::initPersistFields()
{
    addField("rotatation", TypeF32, Offset(mRotation, GuiRadarChartCtrl), "rotation");
    addField("numberOfSides", TypeU32, Offset(mNumberOfSides, GuiRadarChartCtrl), "Number of sides");
    addField("outerColor", TypeColorI, Offset(mOuterColor, GuiRadarChartCtrl), "Outer color");
    addField("outerWidth", TypeF32, Offset(mOuterWidth, GuiRadarChartCtrl), "Outer width");
    INITPERSISTFIELD_IMAGEASSET(OuterBitmap, GuiRadarChartCtrl, The bitmap file to display the outer edges.);

    Parent::initPersistFields();
}

void GuiRadarChartCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    F32 angleStep = Float_2Pi / mNumberOfSides;

    const Point2I& currentExtent = getExtent();

    //GFX->getDrawUtil()->draw2DSquare(Point2F(offset.x, offset.y), 100.0f, 3.14f / 2);

    // Calculate all the points
    Vector<Point3F> vertices;
    const Point2I centerPoint = offset + (currentExtent / 2);
    for (U32 iteration = 0; iteration < mNumberOfSides; ++iteration)
    {
        // Calculate current and next points
        F32 currentAngle = (angleStep * iteration) + mRotation;
        F32 currentX = cos(currentAngle) * (currentExtent.x / 2);
        F32 currentY = sin(currentAngle) * (currentExtent.y / 2);

        vertices.push_back(Point3F(currentX + centerPoint.x, currentY + centerPoint.y, 0.0f));
    }

    // Render inner
    GFXStateBlockDesc description;

    GFX->getDrawUtil()->drawPolygon(description, vertices.address(), mNumberOfSides, ColorI::GREEN);
    //void GFXDrawUtil::drawPolygon( const GFXStateBlockDesc& desc, const Point3F* points, U32 numPoints, const ColorI& color, const MatrixF* xfm /* = NULL */ )

    // Render outside edges
    GFX->getDrawUtil()->clearBitmapModulation();
    GFX->getDrawUtil()->setBitmapModulation(mOuterColor);
    for (U32 iteration = 0; iteration < mNumberOfSides; ++iteration)
    {
        U32 nextIndex = (iteration + 1) % mNumberOfSides;

        const Point3F& currentVertex = vertices[iteration];
        const Point3F& nextVertex = vertices[nextIndex];

        GFX->getDrawUtil()->drawLineWidthTextured(currentVertex.x, currentVertex.y, 0.0f,
                                          nextVertex.x, nextVertex.y, 0.0f,
                                          mOuterColor, mOuterWidth, mOuterBitmap);
    }

    Parent::onRender(offset, updateRect);
}