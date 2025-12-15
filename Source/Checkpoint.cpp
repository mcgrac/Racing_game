#include "Checkpoint.h"

Checkpoint::Checkpoint(Module* _listener, const Vector2D& pos, float _rotation, EntityType _type, int _id, uint16 category, uint16 maskBits, int16 groupIndex)
	: Entity(_listener, pos, _type), id(_id), rotation(_rotation)
{
    InitPhysics(category, maskBits, groupIndex);
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateChainSensor(
        position.getX(),
        position.getY(),
        200.0f,
        40.0f,
        rotation,
        b2_staticBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {
        //set fixture
        b2Fixture* fixture = physBody->body->GetFixtureList();
        if (fixture) {
            fixture->SetDensity(1.0f); //density (mass)
        }
        physBody->body->ResetMassData();
        // OnCollision I will be able to do->
        // Entity* entity = reinterpret_cast<Entity*>(body->GetUserData().pointer);

        physBody->listener = listener;

        //save reference in the phys body
        physBody->entity = this;
    }
}
