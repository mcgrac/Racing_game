#include "Boost.h"

Boost::Boost(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex) :
	Interactables(_listener, pos, _type, category, maskBits)

{
    //LoadAnimations();
	InitPhysics(category, maskBits, groupIndex);
}

Boost::~Boost()
{
}

bool Boost::Update(float dt)
{
    //update animations
    return true;
}

bool Boost::Render()
{
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
        position.getX() /*+ (width / 2)*/,
        position.getY() /*+ (height / 2)*/,
        50.0f,
        50.0f,
        b2_dynamicBody,
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

void Boost::LoadAnimations()
{
    //load texture and set width and height with the texture
    anim.AddTexture("---------");

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
