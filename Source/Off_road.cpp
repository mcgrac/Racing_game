#include "Off_road.h"

Off_road::Off_road(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int* points, int size, int16 groupIndex):
    Entity(_listener, pos, _type)
{
	Initialize(category, maskBits, groupIndex, points, size);
}

Off_road::~Off_road()
{
}

void Off_road::Initialize(uint16 category, uint16 maskBits, int16 groupIndex, int* points, int size)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreatePolygonSensor(
        position.getX(),
        position.getY(),
        points,
        size,
        b2_dynamicBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {

        physBody->listener = listener;
        physBody->entity = this;
    }

}
