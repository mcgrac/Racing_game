#include "SolarBeam.h"

SolarBeam::SolarBeam(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, float _rotation, int16 groupIndex) :
    Entity(_listener, pos, _type), rotationBeam(_rotation)
{
    LoadAnimations();
    InitPhysics(category, maskBits, groupIndex);
}

SolarBeam::~SolarBeam()
{
    CleanUp();
}

bool SolarBeam::Update(float dt)
{
    solarBeam.Update(dt);

    // Actualizar rotación del PhysBody
    if (physBody && physBody->body) {
        physBody->body->SetTransform(
            physBody->body->GetPosition(),
            rotationBeam * DEG2RAD  // Convertir grados a radianes
        );
    }

    return true;
}

bool SolarBeam::Render()
{
    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);
    float physRotation = physBody->body->GetAngle() * RAD2DEG;

    Texture2D currentTexture = solarBeam.GetCurrentTexture();

    Rectangle sourceRect = {
        0, 0,
        (float)currentTexture.width,
        -(float)currentTexture.height
    };

    Rectangle destRect = {
        drawX,
        drawY,
        width,
        height
    };

    Vector2 origin = { width * 0.5f, height * 0.5f };

    DrawTexturePro(currentTexture,
        sourceRect,
        destRect,
        origin,
        physRotation,
        WHITE);

    return true;
}

bool SolarBeam::CleanUp()
{
    //delete physbody
    if (physBody)
    {
        listener->App->physics->DestroyPhysBody(physBody);
        physBody = nullptr;
    }
    return true;
}

void SolarBeam::LoadAnimations()
{
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam1.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam2.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam3.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam4.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam5.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam6.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam7.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam8.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam9.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam10.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam11.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam12.png");
    solarBeam.AddTexture("Assets/Textures/Cars/SolarBeam13.png");

    // Obtain size textures
    if (solarBeam.IsValid()) {
        Texture2D firstTex = solarBeam.GetCurrentTexture();
        width = firstTex.width;
        height = firstTex.height;
        std::cout << "Player animations loaded (size: %dx%d)" << (int)width << ", " << (int)height << std::endl;
    }
    else {
        std::cout << "Error getting size animations" << std::endl;
        width = 64;
        height = 64;
    }
}
void SolarBeam::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateRectangleSensor(
        position.getX() + (width / 2),
        position.getY() + height,
        width,
        height,
        rotationBeam,
        b2_dynamicBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {
        physBody->listener = listener;
        //save player's reference in the phys body
        physBody->entity = this;
    }
}
void SolarBeam::SetPositionAndRotation(const Vector2D& carPos, float carRotation)
{
    rotationBeam = carRotation;

    if (physBody && physBody->body) {
        // Convertir rotación a radianes
        float radians = carRotation * DEG2RAD;

        // Calcular el vector forward del coche
        float forwardX = sin(radians);
        float forwardY = -cos(radians);

        // Distancia desde el coche hasta el centro del rayo
        // (mitad de la altura del rayo para que la base esté en el coche)
        float rayOffset = height * 0.5f;

        // Calcular posición del centro del rayo
        float beamCenterX = carPos.getX() + (forwardX * rayOffset);
        float beamCenterY = carPos.getY() + (forwardY * rayOffset);

        // Actualizar posición para el renderizado
        position.setX(beamCenterX - width * 0.5f);
        position.setY(beamCenterY - height * 0.5f);

        // Convertir a metros y actualizar PhysBody
        b2Vec2 physPos;
        physPos.x = PIXEL_TO_METERS(beamCenterX);
        physPos.y = PIXEL_TO_METERS(beamCenterY);

        physBody->body->SetTransform(physPos, radians);
    }
}
