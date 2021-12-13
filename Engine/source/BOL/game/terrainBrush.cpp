#include "BOL/game/terrainBrush.h"

IMPLEMENT_CO_NETOBJECT_V1(TerrainBrush);

ConsoleDocClass( TerrainBrush,
     "@brief A TerrainBrush allows script access to terrain data and networked deformation.\n\n"
);

void TerrainBrush::initPersistFields()
{
    Parent::initPersistFields();
}

U32 TerrainBrush::packUpdate(NetConnection *conn, U32 mask, BitStream* stream)
{
    return Parent::packUpdate(conn, mask, stream);
}

void TerrainBrush::unpackUpdate(NetConnection *conn, BitStream* stream)
{
    Parent::unpackUpdate(conn, stream);
}

void TerrainBrush::adjustHeight(F32 height)
{

}

void TerrainBrush::flatten()
{

}

void TerrainBrush::paintNoise(F32 heightVariation, S32 seed)
{

}

/*
DefineEngineMethod( TerrainBrush, getMaximumHeight, F32, ( ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@returns The average terrain height at the world position.\n")
{
    return object->getMaximumTerrainHeight();
}

DefineEngineMethod( TerrainBrush, getMinimumHeight, F32, ( ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@returns The average terrain height at the world position.\n")
{
    return object->getMinimumTerrainHeight();
}

DefineEngineMethod( TerrainBrush, getAverageHeight, F32, ( ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@returns The average terrain height at the world position.\n")
{
    return object->getAverageTerrainHeight();
}
*/

DefineEngineMethod( TerrainBrush, adjustHeight, void, ( F32 height ),,
                    "@brief Retrieve the average terrain height at the current world position.\n\n"
                    "@param height The height to lower by.\n"
                    "@returns The average terrain height at the world position.\n")
{
    object->adjustHeight(height);
}

DefineEngineMethod( TerrainBrush, flatten, void, ( ),,
                    "@brief Flatten the terrain at the specified position.")
{
    object->flatten();
}

DefineEngineMethod( TerrainBrush, paintNoise, void, ( F32 heightVariation, S32 seed ),,
                    "@brief Flatten the terrain at the specified position.")
{
    object->paintNoise(heightVariation, seed);
}

//--------------------------------------------------------------------------

IMPLEMENT_CO_NETOBJECT_V1(RectangleTerrainBrush);

ConsoleDocClass( RectangleTerrainBrush,
                 "@brief A TerrainBrush allows script access to terrain data and networked deformation.\n\n"
);

U32 RectangleTerrainBrush::packUpdate(NetConnection *conn, U32 mask, BitStream* stream)
{
    stream->writeFloat(mBrush.mWidth, 10);
    stream->writeFloat(mBrush.mHeight, 10);
    return Parent::packUpdate(conn, mask, stream);
}

void RectangleTerrainBrush::unpackUpdate(NetConnection *conn, BitStream* stream)
{
    mBrush.mWidth = stream->readFloat(10);
    mBrush.mHeight = stream->readFloat(10);
    Parent::unpackUpdate(conn, stream);
}

void RectangleTerrainBrush::initPersistFields()
{
    addField("width", TypeF32, Offset(mBrush.mWidth, RectangleTerrainBrush), "The width of the brush.");
    addField("height", TypeF32, Offset(mBrush.mHeight, RectangleTerrainBrush), "The height of the brush.");

    Parent::initPersistFields();
}

void RectangleTerrainBrush::adjustHeight(F32 height)
{
    AdjustHeightAction* newAction = new AdjustHeightAction(height, new RectangleDeformationBrush(mBrush));
    DeformationManager* manager = DeformationManager::getDeformationManager();

    manager->registerAction(newAction);
}

void RectangleTerrainBrush::flatten()
{
    FlattenAction* newAction = new FlattenAction(new RectangleDeformationBrush(mBrush));
    DeformationManager* manager = DeformationManager::getDeformationManager();

    manager->registerAction(newAction);
}

void RectangleTerrainBrush::paintNoise(F32 heightVariation, S32 seed)
{

}
