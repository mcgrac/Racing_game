#include "Checkpoint.h"

Checkpoint::Checkpoint(Module* _listener, const Vector2D& pos, EntityType _type, int _id, uint16 category, uint16 maskBits, int16 groupIndex, const int* points, int size)
	: Entity(_listener, pos, _type), id(_id)
{
    InitPhysics(category, maskBits, groupIndex, points, size);
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex, const int* points, int size)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateChainSensor(
        position.getX(),
        position.getY(),
        points,
        size,
        b2_dynamicBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {
        //set body
        physBody->body->SetLinearDamping(0.3f);   // air friction
        physBody->body->SetAngularDamping(3.0f);  // resistence to turning
        //set fixture
        b2Fixture* fixture = physBody->body->GetFixtureList();
        if (fixture) {
            fixture->SetDensity(1.2f); //density (mass)
            fixture->SetFriction(0.4f); //friction with the floor
            fixture->SetRestitution(0.2f); //doesn't bounce
        }

        physBody->body->ResetMassData(); //Necessary for recalculation of mass, centre of mass etc...

        // OnCollision I will be able to do->
        // Entity* entity = reinterpret_cast<Entity*>(body->GetUserData().pointer);

        physBody->listener = listener;

        //save player's reference in the phys body
        physBody->entity = this;
    }

    // already done in Entity, just for security

}