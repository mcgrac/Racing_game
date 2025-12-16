#include "AttackPachirisu.h"

AttackPachirisu::AttackPachirisu(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex) : 
    Entity(_listener, pos, _type)
{
    LoadAnimations();
    InitPhysics(category, maskBits, groupIndex);
}

AttackPachirisu::~AttackPachirisu()
{
    CleanUp();
}

bool AttackPachirisu::Update(float dt)
{
    discharge.Update(dt);
	return true;
}

bool AttackPachirisu::Render()
{
    // Obtener posición y rotación del cuerpo físico
    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);

    // Obtener la textura actual según el estado
    Texture2D currentTexture = discharge.GetCurrentTexture();

    // Rectángulo de origen (toda la textura)
    Rectangle sourceRect = {
        0, 0,
        (float)currentTexture.width,
        -(float)currentTexture.height
    };

    // Rectángulo de destino
    Rectangle destRect = {
        drawX,
        drawY,
        width,
        height
    };

    // Origen para la rotación (centro)
    Vector2 origin = { width * 0.5f, height * 0.5f };

    DrawTexturePro(currentTexture,
        sourceRect,
        destRect,
        origin,
        0.0f,
        WHITE);

    return true;
}

bool AttackPachirisu::CleanUp()
{
    //delete physbody
    if(physBody)
    {
        listener->App->physics->DestroyPhysBody(physBody);
        physBody = nullptr;
    }
	return true;
}

void AttackPachirisu::LoadAnimations()
{
    discharge.AddTexture("Assets/Textures/Cars/Discharge1.png");
    discharge.AddTexture("Assets/Textures/Cars/Discharge2.png");

    // Obtain size textures
    if (discharge.IsValid()) {
        Texture2D firstTex = discharge.GetCurrentTexture();
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
void AttackPachirisu::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateCircleSensor(
        position.getX() + (width / 2),
        position.getY() + (height / 2),
        width * 0.75f,
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

