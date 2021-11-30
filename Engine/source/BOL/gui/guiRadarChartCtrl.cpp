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
    mChartDivisor = 2.0f;
    mOuterColor = ColorI::RED;
    mInnerColor = ColorI::GREEN;
    mDataSetColor = ColorI::BLUE;

    INIT_ASSET(InnerBitmap);
    INIT_ASSET(OuterBitmap);
    INIT_ASSET(DataSetBitmap);

    for (U32 iteration = 0; iteration < maxSides; ++iteration)
    {
        mVertexStrengths[iteration] = 0.0f;
        mVertexTexts[iteration] = StringTable->EmptyString();
    }
}

void GuiRadarChartCtrl::initPersistFields()
{
    addGroup("Chart");
        addField("rotatation", TypeF32, Offset(mRotation, GuiRadarChartCtrl), "rotation");
        addField("numberOfSides", TypeU32, Offset(mNumberOfSides, GuiRadarChartCtrl), "Number of sides");
        addField("outerColor", TypeColorI, Offset(mOuterColor, GuiRadarChartCtrl), "Outer color");
        addField("innerColor", TypeColorI, Offset(mInnerColor, GuiRadarChartCtrl), "Inner color");
        addField("dataSetColor", TypeColorI, Offset(mDataSetColor, GuiRadarChartCtrl), "Data set color");
        addField("outerWidth", TypeF32, Offset(mOuterWidth, GuiRadarChartCtrl), "Outer width");
        addField("chartDivisor", TypeF32, Offset(mChartDivisor, GuiRadarChartCtrl), "Chart divisor - used for scaling");
        addField("vertexStrengths", TypeF32, Offset(mVertexStrengths, GuiRadarChartCtrl), maxSides, "Vertex strengths.");
        addField("vertexTexts", TypeString, Offset(mVertexTexts, GuiRadarChartCtrl), maxSides, "Vertex texts");

        INITPERSISTFIELD_IMAGEASSET(OuterBitmap, GuiRadarChartCtrl, The bitmap file to display the outer edges);
        INITPERSISTFIELD_IMAGEASSET(InnerBitmap, GuiRadarChartCtrl, The bitmap file to display in the inside);
        INITPERSISTFIELD_IMAGEASSET(DataSetBitmap, GuiRadarChartCtrl, The bitmap file to display on the dataset);
    endGroup("Chart");

    Parent::initPersistFields();
}

static inline void constructPolygonTriangles(const Vector<Point3F>& outerPoints, const Point2I& centerPoint, Vector<Point3F>& result)
{
    for (U32 iteration = 0; iteration < outerPoints.size(); ++iteration)
    {
        U32 nextIndex = (iteration + 1) % outerPoints.size();

        result.push_back(Point3F(centerPoint.x, centerPoint.y, 0.0f));
        result.push_back(outerPoints[iteration]);
        result.push_back(outerPoints[nextIndex]);
    }

    if (outerPoints.size() % 2 == 0)
    {
        result.push_back(Point3F(centerPoint.x, centerPoint.y, 0.0f));
        result.push_back(outerPoints[0]);
        result.push_back(outerPoints[1]);
    }
}

static inline void constructPolygonOuterPoints(U32 numberOfSides, F32 rotation, F32 divisor, const Point2I& centerPoint, const Point2I& extent, Vector<Point3F>& result)
{
    F32 angleStep = Float_2Pi / numberOfSides;
    for (U32 iteration = 0; iteration < numberOfSides; ++iteration)
    {
        // Calculate current and next points
        F32 currentAngle = (angleStep * iteration) + rotation;
        F32 currentX = cos(currentAngle) * (extent.x / divisor);
        F32 currentY = sin(currentAngle) * (extent.y / divisor);

        result.push_back(Point3F(currentX + centerPoint.x, currentY + centerPoint.y, 0.0f));
    }
}

static inline void constructRadiatingPolygonOuterPoints(U32 numberOfSides, F32 rotation, F32 divisor, const Point2I& centerPoint, const Point2I& extent, F32* strengths, U32 strengthCount, Vector<Point3F>& result)
{
    AssertFatal(strengthCount == numberOfSides, "Number of sides and strength value count should be equal!");

    F32 angleStep = Float_2Pi / numberOfSides;
    for (U32 iteration = 0; iteration < numberOfSides; ++iteration)
    {
        AssertFatal(strengths[iteration] >= 0.0f && strengths[iteration] <= 1.0f, "Strength values should be normalized!");

        // Calculate current and next points
        F32 currentAngle = (angleStep * iteration) + rotation;
        F32 currentX = cos(currentAngle) * (extent.x / divisor) * strengths[iteration];
        F32 currentY = sin(currentAngle) * (extent.y / divisor) * strengths[iteration];

        result.push_back(Point3F(currentX + centerPoint.x, currentY + centerPoint.y, 0.0f));
    }
}

void GuiRadarChartCtrl::onRender(Point2I offset, const RectI &updateRect)
{
    const Point2I& currentExtent = getExtent();
    const Point2I centerPoint = offset + (currentExtent / 2);

    Vector<Point3F> outerVertices;
    constructPolygonOuterPoints(mNumberOfSides, mRotation, mChartDivisor, centerPoint, currentExtent, outerVertices);
    
    // Render inner filler polygon
    GFXStateBlockDesc description;
    Vector<Point3F> innerTriangles;
    constructPolygonTriangles(outerVertices, centerPoint, innerTriangles);
    GFX->getDrawUtil()->drawPolygonTexture(description, innerTriangles.address(), innerTriangles.size(), mInnerColor, NULL, mInnerBitmap);

    // Render outside edges
    GFX->getDrawUtil()->clearBitmapModulation();
    GFX->getDrawUtil()->setBitmapModulation(mOuterColor);
    for (U32 iteration = 0; iteration < mNumberOfSides; ++iteration)
    {
        U32 nextIndex = (iteration + 1) % mNumberOfSides;

        const Point3F& currentVertex = outerVertices[iteration];
        const Point3F& nextVertex = outerVertices[nextIndex];

        GFX->getDrawUtil()->drawLineWidthTextured(currentVertex.x, currentVertex.y, 0.0f,
                                                  nextVertex.x, nextVertex.y, 0.0f,
                                                  mOuterColor, mOuterWidth, mOuterBitmap);
    }

    // Render data set
    Vector<Point3F> dataSetOuterVertices;
    constructRadiatingPolygonOuterPoints(mNumberOfSides, mRotation, mChartDivisor, centerPoint, currentExtent, mVertexStrengths, mNumberOfSides, dataSetOuterVertices);

    Vector<Point3F> dataSetTriangles;
    constructPolygonTriangles(dataSetOuterVertices, centerPoint, dataSetTriangles);

    GFX->getDrawUtil()->drawPolygonTexture(description, dataSetTriangles.address(), dataSetTriangles.size(), mDataSetColor, NULL, mDataSetBitmap);

    // Render text on each point
    for (U32 iteration = 0; iteration < mNumberOfSides; ++iteration)
    {
        if (mVertexTexts[iteration] != StringTable->EmptyString())
        {
            const Point3F& currentVertex = outerVertices[iteration];

            renderJustifiedText(Point2I(currentVertex.x, currentVertex.y), Point2I(25, 25), mVertexTexts[iteration]);
        }
    }

    Parent::onRender(offset, updateRect);
}