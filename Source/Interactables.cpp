#include "Interactables.h"

Interactables::Interactables(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex)
    : Entity(_listener, pos, _type)
{
}


Interactables::~Interactables()
{
}

void Interactables::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
}

void Interactables::LoadAnimations()
{
}
