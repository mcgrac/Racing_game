#include "ChanseyAttack.h"

ChanseyAttack::ChanseyAttack(Module* _listener, const Vector2D& pos, float carRotation, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex) :
    Entity(_listener, pos, _type)
{
    LoadAnimations();
    InitPhysics(category, maskBits, groupIndex);
    ThrowEgg(carRotation);
}

ChanseyAttack::~ChanseyAttack()
{
    CleanUp();
}

bool ChanseyAttack::Update(float dt)
{
    switch (currentState)
    {
    case EggState::FLYING:
    {
        egg.Update(dt);
        flyingTimer += dt;

        // Verify if max time flying is over
        if (flyingTimer >= maxFlyingTime)
        {
            BreakEgg();
        }

        // If velocity is very low -> egg has touched the floor
        if (physBody && physBody->body)
        {
            b2Vec2 velocity = physBody->body->GetLinearVelocity();
            float speed = velocity.Length();

            // If it has been stop nearly for complete, break the egg
            if (speed < 0.5f && flyingTimer > 0.5f)
            {
                BreakEgg();
            }
        }
        break;
    }

    case EggState::BROKEN:
    {
        brokenEgg.Update(dt);
        brokenTimer += dt;

        // After x seconds set it to dissappear
        if (brokenTimer >= brokenDuration)
        {
            currentState = EggState::DISAPPEARED;
        }
        break;
    }

    case EggState::DISAPPEARED:
        //do nothing
        break;
    }

    return true;

}

bool ChanseyAttack::Render()
{
    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);
    float physRotation = physBody->body->GetAngle() * RAD2DEG;

    Texture2D currentTexture;

    // sewlect texture and behaviour depending in the state
    if (currentState == EggState::FLYING)
    {
        currentTexture = egg.GetCurrentTexture();
        // Egg rotates while flying
        float physRotation = physBody->body->GetAngle() * RAD2DEG;

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
    }
    else if (currentState == EggState::BROKEN)
    {
        currentTexture = brokenEgg.GetCurrentTexture();

        Rectangle sourceRect = {
            0, 0,
            (float)currentTexture.width,
            -(float)currentTexture.height
        };

        Rectangle destRect = {
            drawX,
            drawY,
            width * 1.2f,  // a little bit bigger when it is broken
            height * 1.2f
        };

        Vector2 origin = { (width * 1.2f) * 0.5f, (height * 1.2f) * 0.5f };

        DrawTexturePro(currentTexture,
            sourceRect,
            destRect,
            origin,
            0.0f,
            WHITE);
    }

    return true;
}

bool ChanseyAttack::CleanUp()
{
    //delete physbody
    if (physBody)
    {
        listener->App->physics->DestroyPhysBody(physBody);
        physBody = nullptr;
    }
    return true;
}

void ChanseyAttack::LoadAnimations()
{
    egg.AddTexture("Assets/Textures/Cars/Egg.png");

    brokenEgg.AddTexture("Assets/Textures/Cars/Egg2.png");

    // Obtain size textures
    if (egg.IsValid()) {
        Texture2D firstTex = egg.GetCurrentTexture();
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
void ChanseyAttack::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateCircleSensor(
        position.getX() + (width / 2),
        position.getY() + (height / 2),
        width / 2,
        b2_dynamicBody,
        category,
        maskBits,
        groupIndex);

    if (physBody && physBody->body) {

        b2Fixture* fixture = physBody->body->GetFixtureList();
        if (fixture) {
            fixture->SetDensity(0.8f);      
            fixture->SetFriction(0.6f);     
            fixture->SetRestitution(0.4f);  
        }

        physBody->body->ResetMassData();
        physBody->body->SetLinearDamping(1.5f);   
        physBody->body->SetAngularDamping(0.3f);  

        physBody->listener = listener;
        physBody->entity = this;
    }
}

void ChanseyAttack::ThrowEgg(float carRotation)
{
    if (!physBody || !physBody->body)
    {
        return;
    }

    float radians = carRotation * DEG2RAD;

    float backwardX = -sin(radians);
    float backwardY = cos(radians);

    b2Vec2 impulse(backwardX * throwForce, backwardY * throwForce);
    physBody->body->ApplyLinearImpulseToCenter(impulse, true);

    float randomTorque = (rand() % 100 - 50) * 0.1f;
    physBody->body->ApplyTorque(randomTorque, true);
}

void ChanseyAttack::BreakEgg()
{
    if (currentState != EggState::FLYING)
    {
        return;
    }

    std::cout << "Egg broken!" << std::endl;

    currentState = EggState::BROKEN;
    brokenTimer = 0.0f;

    if (physBody && physBody->body)
    {
        physBody->body->SetLinearVelocity(b2Vec2(0, 0));
        physBody->body->SetAngularVelocity(0.0f);

        physBody->body->SetType(b2_staticBody);
    }
}
