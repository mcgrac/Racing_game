#include "Boost.h"

Boost::Boost(Module* _listener, const Vector2D& pos, float _rotation, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex) :
	Interactables(_listener, pos, _type, category, maskBits), rotation(_rotation * DEG2RAD)

{
    
    LoadAnimations();
	InitPhysics(category, maskBits, groupIndex);
}

Boost::~Boost()
{
}

bool Boost::Update(float dt)
{
    anim.Update(dt);
    return true;
}

bool Boost::Render()
{
    // Obtener posición y rotación del cuerpo físico
    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);

    // Obtener la textura actual según el estado
    Texture2D currentTexture = anim.GetCurrentTexture();

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

    Vector2 origin = { width * 0.5f, height * 0.5f };

    float rotationDeg = rotation * RAD2DEG;

    DrawTexturePro(currentTexture,
        sourceRect,
        destRect,
        origin,
        rotationDeg,
        WHITE);

    //render
    return true;
}

void Boost::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
	if (!listener->App->physics) {
		LOG("ERROR: Physics module is null!");
		return;
	}

    physBody = listener->App->physics->CreateRectangleSensor(
        position.getX(),
        position.getY(),
        50.0f,
        50.0f,
        0.0f,
        b2_staticBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {

        physBody->body->SetTransform(
            physBody->body->GetPosition(),
            rotation //in radians
        );

        physBody->listener = listener;
        physBody->entity = this;
    }
}

void Boost::LoadAnimations()
{
    //load texture and set width and height with the texture
    anim.AddTexture("Assets/Textures/Map/ArrowAnim31.png");
    anim.AddTexture("Assets/Textures/Map/ArrowAnim32.png");
    anim.AddTexture("Assets/Textures/Map/ArrowAnim33.png");
    anim.AddTexture("Assets/Textures/Map/ArrowAnim34.png");

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
