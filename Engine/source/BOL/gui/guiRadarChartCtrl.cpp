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
    mRotation = 0.0f;

    for (U32 iteration = 0; iteration < maxSides; ++iteration)
    {
        mVertexValues[iteration] = 50.0f;
        mVertexMaxValues[iteration] = 100.0f;
        mVertexColors[iteration] = ColorI::WHITE;
        mVertexTexts[iteration] = StringTable->EmptyString();
    }
}

void GuiRadarChartCtrl::initPersistFields()
{
    addGroup("Chart");
        addField("rotatation", TypeF32, Offset(mRotation, GuiRadarChartCtrl), "rotation");
        addField("rotatation", TypeF32, Offset(mRotation, GuiRadarChartCtrl), "rotation");
        addField("vertexValues", TypeF32, Offset(mVertexValues, GuiRadarChartCtrl), maxSides, "Vertex strengths.");
        addField("vertexMaxValues", TypeF32, Offset(mVertexMaxValues, GuiRadarChartCtrl), maxSides, "Vertex max values.");
        addField("vertexTexts", TypeString, Offset(mVertexTexts, GuiRadarChartCtrl), maxSides, "Vertex texts");
        addField("vertexColors", TypeColorI, Offset(mVertexColors, GuiRadarChartCtrl), maxSides, "Vertex colors.");
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
    GuiRadarChartCtrlProfile* profile = dynamic_cast<GuiRadarChartCtrlProfile*>(mProfile);
    if (!profile)
    {
        return;
    }

    const Point2I& currentExtent = getExtent();
    const Point2I centerPoint = offset + (currentExtent / 2);

    Vector<Point3F> outerVertices;
    constructPolygonOuterPoints(profile->mNumberOfSides, mRotation, profile->mChartDivisor, centerPoint, currentExtent, outerVertices);
    
    // Render inner filler polygon
    GFXStateBlockDesc description;
    Vector<Point3F> innerTriangles;
    constructPolygonTriangles(outerVertices, centerPoint, innerTriangles);
    GFX->getDrawUtil()->drawPolygonTexture(description,
                                           innerTriangles.address(),
                                           innerTriangles.size(),
                                           NULL,
                                           GFXDrawUtil::UVMode::RadialMap,
                                           profile->mInnerColor,
                                           NULL,
                                           profile->mInnerBitmap);

    // Render outside edges
    GFX->getDrawUtil()->clearBitmapModulation();
    GFX->getDrawUtil()->setBitmapModulation(profile->mOuterColor);
    for (U32 iteration = 0; iteration < profile->mNumberOfSides; ++iteration)
    {
        U32 nextIndex = (iteration + 1) % profile->mNumberOfSides;

        const Point3F& currentVertex = outerVertices[iteration];
        const Point3F& nextVertex = outerVertices[nextIndex];

        GFX->getDrawUtil()->drawLineWidthTextured(currentVertex.x, currentVertex.y, 0.0f,
                                                  nextVertex.x, nextVertex.y, 0.0f,
                                                  profile->mOuterColor, profile->mOuterWidth, profile->mOuterBitmap);
    }

    // Render data set
    Vector<F32> dataSetStrengths;
    for (U32 iteration = 0; iteration < profile->mNumberOfSides; ++iteration)
    {
        const F32 sideValue = std::min(mVertexMaxValues[iteration], mVertexValues[iteration]);
        dataSetStrengths.push_back(sideValue /  mVertexMaxValues[iteration]);
    }

    Vector<Point3F> dataSetOuterVertices;
    constructRadiatingPolygonOuterPoints(profile->mNumberOfSides,
                                         mRotation,
                                         profile->mChartDivisor,
                                         centerPoint,
                                         currentExtent,
                                         dataSetStrengths.address(),
                                         profile->mNumberOfSides,
                                         dataSetOuterVertices);

    Vector<Point3F> dataSetTriangles;
    constructPolygonTriangles(dataSetOuterVertices, centerPoint, dataSetTriangles);

    GFX->getDrawUtil()->drawPolygonTexture(description,
                                           dataSetTriangles.address(),
                                           dataSetTriangles.size(),
                                           mVertexColors,
                                           GFXDrawUtil::UVMode::RadialMap,
                                           ColorI::BLACK,
                                           NULL,
                                           profile->mDataSetBitmap);

    // Render values on each data set point
    for (U32 iteration = 0; iteration < profile->mNumberOfSides; ++iteration)
    {
        char valueString[32];
        dSprintf(valueString, sizeof(valueString), "%f", mVertexValues[iteration]);

        const Point3F& currentVertex = dataSetOuterVertices[iteration];
        renderJustifiedText(Point2I(currentVertex.x, currentVertex.y), Point2I(25, 25), valueString);
    }

    // Render text on each outer point
    for (U32 iteration = 0; iteration < profile->mNumberOfSides; ++iteration)
    {
        if (mVertexTexts[iteration] != StringTable->EmptyString())
        {
            const Point3F& currentVertex = outerVertices[iteration];

            renderJustifiedText(Point2I(currentVertex.x, currentVertex.y), Point2I(25, 25), mVertexTexts[iteration]);
        }
    }

    Parent::onRender(offset, updateRect);
}


//-----------------------------------------------------------------------------
// Console stuff (GuiRadarChartCtrlProfile)
//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(GuiRadarChartCtrlProfile);

ConsoleDocClass( GuiRadarChartCtrlProfile,
 "@brief A GuiControlProfile with additional fields specific to GuiRadarChartCtrl.\n\n"

     "@ingroup GuiGame"
);

GuiRadarChartCtrlProfile::GuiRadarChartCtrlProfile() : GuiControlProfile()
{
    mOuterWidth = 10.0f;
    mChartDivisor = 2.0f;
    mShowValues = true;
    mOuterColor = ColorI::RED;
    mInnerColor = ColorI::GREEN;
    mNumberOfSides = 4;

    INIT_ASSET(InnerBitmap);
    INIT_ASSET(OuterBitmap);
    INIT_ASSET(DataSetBitmap);
}

void GuiRadarChartCtrlProfile::initPersistFields()
{
    addGroup("Chart");
        addField("numberOfSides", TypeU32, Offset(mNumberOfSides, GuiRadarChartCtrlProfile), "Number of sides");
        addField("outerColor", TypeColorI, Offset(mOuterColor, GuiRadarChartCtrlProfile), "Outer color");
        addField("innerColor", TypeColorI, Offset(mInnerColor, GuiRadarChartCtrlProfile), "Inner color");
        addField("outerWidth", TypeF32, Offset(mOuterWidth, GuiRadarChartCtrlProfile), "Outer width");
        addField("chartDivisor", TypeF32, Offset(mChartDivisor, GuiRadarChartCtrlProfile), "Chart divisor - used for scaling");
        addField("showValues", TypeBool, Offset(mShowValues, GuiRadarChartCtrlProfile), "Show values at each point of the data set.");

        INITPERSISTFIELD_IMAGEASSET(OuterBitmap, GuiRadarChartCtrlProfile, The bitmap file to display the outer edges);
        INITPERSISTFIELD_IMAGEASSET(InnerBitmap, GuiRadarChartCtrlProfile, The bitmap file to display in the inside);
        INITPERSISTFIELD_IMAGEASSET(DataSetBitmap, GuiRadarChartCtrlProfile, The bitmap file to display on the dataset);
    endGroup("Chart");

    Parent::initPersistFields();
}
