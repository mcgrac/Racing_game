#include "Rock.h"

Rock::Rock(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex):
	Interactables(_listener, pos, _type, category, maskBits)
{
    LoadAnimations();
	InitPhysics(category, maskBits, groupIndex);

    idle = LoadTexture("Assets/Textures/Map/BreakableRock.png");
}

Rock::~Rock()
{

}

bool Rock::Update(float dt)
{
    if (isDestroyed) {
        anim.Update(dt);
        if (anim.GetCurrentFrame() == 4) {
            Destroy();
        }
    }
	return true;
}

bool Rock::Render()
{
    if (!isDestroyed) {
        DrawTexture(idle, position.getX(), position.getY(), WHITE);
        return true;
    }

    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);

    Texture2D currentTexture = anim.GetCurrentTexture();

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

    Vector2 origin = { width * 0.5f, height * 0.5f };


    DrawTexturePro(currentTexture,
        sourceRect,
        destRect,
        origin,
        0.0f,
        WHITE);

    //render
    return true;
}

bool Rock::Destroy()
{
    pendingToDelete = true;
    active = false;
    return true;
}

bool Rock::CleanUp()
{
    UnloadTexture(idle);
    listener->App->physics->DestroyPhysBody(physBody);
    return true;
}

void Rock::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateCircle(
        position.getX() + width/2.0f,
        position.getY() + height/2.0f,
        width / 2.0f,
        b2_staticBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {

        physBody->listener = listener;
        physBody->entity = this;
    }
}

void Rock::LoadAnimations()
{
    //load texture and set width and height with the texture
    anim.AddTexture("Assets/Textures/Map/BreakableRock.png");
    anim.AddTexture("Assets/Textures/Map/BreakableRock1.png");
    anim.AddTexture("Assets/Textures/Map/BreakableRock2.png");
    anim.AddTexture("Assets/Textures/Map/BreakableRock3.png");
    anim.AddTexture("Assets/Textures/Map/BreakableRock4.png");

    if (anim.IsValid()) {
        Texture2D firstTex = anim.GetCurrentTexture();
        width = firstTex.width;
        height = firstTex.height;
        std::cout << "Boosts animations loaded (size: %dx%d)" << (int)width << ", " << (int)height << std::endl;
    }
    else {
        std::cout << "Error getting size animations" << std::endl;
        width = 64;
        height = 64;
    }
}
