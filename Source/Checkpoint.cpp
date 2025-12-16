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

        //calculate the geometric center of the chain
        float sumX = 0.0f;
        float sumY = 0.0f;
        int numPoints = size / 2; // size es el número total de INTs (x, y, x, y...)

        // Iterar sobre los puntos (cada par de ints es un punto)
        for (int i = 0; i < numPoints; ++i)
        {
            sumX += (float)points[i * 2];     // Coordenada X
            sumY += (float)points[i * 2 + 1]; // Coordenada Y
        }

        if (numPoints > 0)
        {
            // Calcular el promedio (centroide)
            float centerX = sumX / (float)numPoints;
            float centerY = sumY / (float)numPoints;

            // Almacenar el resultado en la variable de la clase
            center.setX(centerX);
            center.setY(centerY);

            // Opcional: Mostrar la posición del centroide para debug
            // LOG("Checkpoint %d Center: (%.1f, %.1f)", id, centerX, centerY);
        }
        else
        {
            // Si no hay puntos, el centro es (0,0) o la posición de la entidad
            center.setX(position.getX());
            center.setY(position.getY());
        }
    }

    // already done in Entity, just for security

}