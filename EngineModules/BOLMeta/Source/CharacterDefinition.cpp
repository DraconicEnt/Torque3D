#include "CharacterDefinition.h"

IMPLEMENT_CONOBJECT(CharacterDefinition);

ConsoleDocClass( CharacterDefinition,
   "@brief A metadata structure for characters.\n\n"
);

CharacterDefinition::CharacterDefinition() : ScriptObject()
{
    INIT_ASSET(TestBitMap);
}

void CharacterDefinition::initPersistFields()
{
    INITPERSISTFIELD_IMAGEASSET(TestBitMap, CharacterDefinition, The bitmap file to display in the control);

    Parent::initPersistFields();
}