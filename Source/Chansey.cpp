#include "Chansey.h"
#include "Globals.h"


Chansey::Chansey(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex)
    : Characters(_listener, startPos, _type, category, maskBits)
{
    LoadAnimations();
    InitPhysics(category, maskBits, groupIndex);

    waypointReachRadius = 50.0f;
    accelerationForce = 65.0f;
    //attack
    attackSound = LoadSound("Assets/Sound/Sfx/Chansey.wav");
    SetSoundVolume(attackSound, 0.4f);
}

Chansey::~Chansey()
{
    
}

void Chansey::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
{
    if (!listener->App->physics) {
        LOG("ERROR: Physics module is null!");
        return;
    }

    physBody = listener->App->physics->CreateRectangle(
        position.getX() + (width / 2),
        position.getY() + (height / 2),
        width,
        height,
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
            fixture->SetDensity(1.0f); //density (mass)
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

void Chansey::SyncPositionFromPhysics()
{
    //meters
    b2Vec2 pos = physBody->body->GetPosition();
    //pixels
    b2Vec2 posPixels;
    posPixels.Set(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y));

    float angle = physBody->body->GetAngle();

    // Convertir metros → píxeles si usas escala
    position.setX(posPixels.x - (width / 2.0f));
    position.setY(posPixels.y - (height / 2.0f));

    rotation = angle * RAD2DEG;
}

// Load assets
bool Chansey::Start()
{
    LOG("Loading player");
    return true;
}

#pragma region LOADING / UNLOADING
void Chansey::LoadAnimations()
{
    //Idle animations
    idleAnimation.AddTexture("Assets/Textures/Cars/CarChanseyIdle1.png");
    idleAnimation.AddTexture("Assets/Textures/Cars/CarChanseyIdle2.png");

    //stunned
    stunnedAnimation.AddTexture("Assets/Textures/Cars/StunnedChansey.png");

    //preparing attack
    preparingAttack.AddTexture("Assets/Textures/Cars/AttackChansey1.png");
    preparingAttack.AddTexture("Assets/Textures/Cars/AttackChansey2.png");
    preparingAttack.AddTexture("Assets/Textures/Cars/AttackChansey3.png");

    // Obtain size textures
    if (idleAnimation.IsValid()) {
        Texture2D firstTex = idleAnimation.GetCurrentTexture();
        width = firstTex.width;
        height = firstTex.height;
        std::cout << "Player animations loaded (size: %dx%d)" << (int)width << ", " << (int)height << std::endl;
        textureLoaded = true;
    }
    else {
        std::cout << "Error getting size animations" << std::endl;
        width = 64;
        height = 64;
    }
}
#pragma endregion

#pragma region ANIMATIONS
void Chansey::UpdateAnims(float dt)
{
    switch (currentState)
    {
    case Characters::State::IDLE:
        idleAnimation.Update(dt);
        break;
    case Characters::State::ATTACK:
        idleAnimation.Update(dt);
        break;
    case Characters::State::STUNNED:
        stunnedAnimation.Update(dt);
        break;
    case Characters::State::PREPARING_ATTACK:
        preparingAttack.Update(dt);
        break;
    default:
        break;
    }
}

#pragma endregion

// Unload assets
bool Chansey::CleanUp()
{
    LOG("Unloading player");

    return true;
}

// Update: draw background
bool Chansey::Update(float dt)
{
    if (IsKeyPressed(KEY_SPACE) && currentState == State::IDLE && isPlayer) {
        Attack();
    }

    UpdateAnims(dt); //update current animation depending on the state

    if (currentState == State::PREPARING_ATTACK) {
        if (preparingAttack.GetCurrentFrame() == 2) {

            preparingAttack.Reset();

            if (!attack)
            {
                attack = new ChanseyAttack(listener, GetCenter(), rotation, EntityType::ATTACK, PhysicCategory::ATTACK, PhysicCategory::AI);
                PlaySound(attackSound);
            }

            currentState = State::ATTACK;
        }
    }

    if (currentState == State::ATTACK) {

        stateTimer += dt;

        if(attack)
        {
            attack->Update(dt);

            // Verificar si el huevo debe ser eliminado
            if (attack->ShouldBeDeleted()) {
                delete attack;
                attack = nullptr;
                currentState = State::IDLE;
                stateTimer = 0.0f;
            }
        }
    }

    if (currentState == State::STUNNED) {
        //be stunned for a while
        if (stateTimer >= 2.0f) {
            maxForwardSpeed = maxForwardSpeed / 2.0f;
        }
        std::cout << "Stunned" << std::endl;
        UpdateState(dt);

    }

    if (isPlayer)
    {
        ApplyCarPhysics(dt);
    }
    else {
        ApplyAIControl(dt);
    }

    SyncPositionFromPhysics();
    return true;
}

#pragma region DRAGS
void Chansey::ApplyDrag()
{
    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();

    // F_drag = -dragCoefficient * currentVelocity
    b2Vec2 dragForce = -dragCoefficient * currentVelocity;

    if (currentVelocity.LengthSquared() > 0.1f)
    {
        body->ApplyForceToCenter(dragForce, true);
    }
    else
    {
        body->SetLinearVelocity(b2Vec2(0, 0));
    }
}
void Chansey::ApplyLateralFriction()
{
    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();

    b2Vec2 rightVector = GetRightVector();

    float lateralSpeed = b2Dot(currentVelocity, rightVector);

    b2Vec2 lateralImpulse = lateralSpeed * rightVector;
    lateralImpulse *= body->GetMass() * lateralDrag;

    body->ApplyLinearImpulse(-lateralImpulse, body->GetWorldCenter(), true);
}
#pragma endregion

#pragma region AI CONTROL
void Chansey::ApplyAIControl(float dt)
{
    if (waypoints.empty()) {
        LOG("WARNING: AI has no waypoints!");
        return;
    }

    b2Body* body = physBody->body;
    b2Vec2 currentVelocity = body->GetLinearVelocity();
    b2Vec2 forwardVector = GetForwardVector();

    float speed = b2Dot(currentVelocity, forwardVector);
    float absoluteSpeed = fabs(speed);

    Vector2D targetWaypoint = GetCurrentWaypoint();
    Vector2D currentPos = GetCenter();

    float distanceToWaypoint = currentPos.distanceEuclidean(targetWaypoint);

    if (distanceToWaypoint < waypointReachRadius) {
        AdvanceToNextWaypoint();
        targetWaypoint = GetCurrentWaypoint();
    }

    float steeringAngle = CalculateSteeringAngle(targetWaypoint);

    bool shouldAccelerate = ShouldAccelerate(targetWaypoint);
    bool shouldBrake = ShouldBrake(targetWaypoint);

    // -------- ACCELERATION --------
    if (shouldAccelerate && !shouldBrake)
    {
        if (speed < maxForwardSpeed)
        {
            b2Vec2 force = (accelerationForce * 0.9f) * forwardVector;
            body->ApplyForceToCenter(force, true);
        }
    }

    // -------- BRAKE --------
    if (shouldBrake && speed > 2.0f)
    {
        b2Vec2 brakeForceVector = -brakeForce * forwardVector;
        body->ApplyForceToCenter(brakeForceVector, true);
    }

    // -------- TURN --------
    if (absoluteSpeed > minSpeedToTurn)
    {
        float turnThreshold = 0.1f;  // ~5.7º

        if (fabs(steeringAngle) > turnThreshold)
        {

            float torqueAmount = steeringAngle * turnTorque;

            if (torqueAmount > turnTorque) torqueAmount = turnTorque;
            if (torqueAmount < -turnTorque) torqueAmount = -turnTorque;

            body->ApplyTorque(torqueAmount, true);
        }
    }

    // -------- PHYSICS --------
    ApplyDrag();
    ApplyLateralFriction();

    this->speed = speed;
}
#pragma region AUXILIARS AI
float Chansey::CalculateSteeringAngle(const Vector2D& targetPos)
{
    Vector2D currentPos = GetCenter();

    Vector2D toTarget = targetPos - currentPos;
    toTarget.normalized();

    b2Vec2 forward = GetForwardVector();

    // dot product: cos(θ) = A·B / (|A||B|)
    float dot = forward.x * toTarget.getX() + forward.y * toTarget.getY();

    // cross_z = Ax*By - Ay*Bx
    float cross = forward.x * toTarget.getY() - forward.y * toTarget.getX();

    float angle = atan2f(cross, dot);

    return angle;
}

bool Chansey::ShouldAccelerate(const Vector2D& targetPos)
{
    Vector2D currentPos = GetCenter();
    float distanceToTarget = currentPos.distanceEuclidean(targetPos);

    float steeringAngle = fabs(CalculateSteeringAngle(targetPos));

    if (steeringAngle > PI / 4.0f) {  // > 45º
        return false;
    }

    if (distanceToTarget > 100.0f) {
        return true;
    }

    if (GetSpeed() < maxForwardSpeed * 0.7f) {
        return true;
    }

    return false;
}

bool Chansey::ShouldBrake(const Vector2D& targetPos)
{
    Vector2D currentPos = GetCenter();
    Vector2D nextWaypoint = GetNextWaypoint();

    float angleToNext = fabs(CalculateSteeringAngle(nextWaypoint));

    if (angleToNext > PI / 3.0f && GetSpeed() > maxForwardSpeed * 0.6f) {  // > 60º
        return true;
    }

    float distanceToTarget = currentPos.distanceEuclidean(targetPos);
    if (distanceToTarget < waypointReachRadius * 1.5f) {
        return true;
    }

    return false;
}
#pragma endregion

#pragma endregion

#pragma region NON AI CONTROL
void Chansey::ApplyCarPhysics(float dt) {

    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();
    b2Vec2 forwardVector = GetForwardVector();

    float speed = b2Dot(currentVelocity, forwardVector);
    float absoluteSpeed = fabs(speed); 

    //-------------------------MOVING FORWARD/BACK--------------------------
    if (IsKeyDown(KEY_W))
    {
        if (speed < maxForwardSpeed)
        {
            // F = m * a
            b2Vec2 force = accelerationForce * forwardVector;
            body->ApplyForceToCenter(force, true);

            if (!IsSoundPlaying(accelerate)) {
                PlaySound(accelerate);
            }

        }
    }
    else if (IsKeyDown(KEY_S))
    {
        if (speed > 0.1f)
        {
            b2Vec2 brakeForceVector = -brakeForce * forwardVector;
            body->ApplyForceToCenter(brakeForceVector, true);
        }
        else if (speed > -maxBackwardSpeed)
        {
            b2Vec2 reverseForce = -accelerationForce * .95f * forwardVector; 
            body->ApplyForceToCenter(reverseForce, true);
        }
    }

    if (IsKeyReleased(KEY_W)) {
        if (IsSoundPlaying(accelerate)) {
            StopSound(accelerate);
        }
    }
    //-------------------------------------------------------------
    //--------------------TURNING RIGHT AND LEFT---------------------
    if (absoluteSpeed > minSpeedToTurn)
    {
        float torqueAmount = 0.0f;

        if (IsKeyDown(KEY_D))
        {
            torqueAmount = +turnTorque;
        }
        else if (IsKeyDown(KEY_A))
        {
            torqueAmount = -turnTorque;
        }

        if (torqueAmount != 0.0f)
        {
            body->ApplyTorque(torqueAmount, true);
        }
    }
    //------------------------------------------------------------
    ApplyDrag();

    ApplyLateralFriction();

    Boost(dt);

    this->speed = speed + turboPower;
}
void Chansey::Attack()
{
    std::cout << "Pressed space key -> attack" << std::endl;
    previousState = currentState;
    currentState = State::PREPARING_ATTACK;
}
#pragma endregion


void Chansey::Boost(float dt)
{
    if (!isBoosted && !isOffRoad) {
        SetMaxSpeed(10.0f); //setVelocity with no boost
        return;
    }

    turboPower -= dt;
    if (turboPower <= 0.0f) {
        isBoosted = false;
        turboPower = 0.0f;
    }
}

bool Chansey::Render() {

    if (attack != nullptr) {
        attack->Render();
    }

    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);

    Texture2D currentTexture;
    switch (currentState) {
    case State::IDLE:
        currentTexture = idleAnimation.GetCurrentTexture();
        break;
    case State::STUNNED:
        currentTexture = stunnedAnimation.GetCurrentTexture();
        break;
    case State::ATTACK:
        currentTexture = idleAnimation.GetCurrentTexture();
        break;
    case State::PREPARING_ATTACK:
        currentTexture = preparingAttack.GetCurrentTexture();
        break;
    default:
        break;
    }

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

    if (textureLoaded) {

        DrawTexturePro(currentTexture,
            sourceRect,
            destRect,
            origin,
            rotation,
            WHITE);
    }

#pragma region DEBUG DRAWING
    // ********** DEBUG: DIBUJAR FORWARD VECTOR *****
// 1. Obtener la posición central y el vector
    b2Vec2 forwardVector = GetForwardVector();
    b2Vec2 positioning = { position.getX() + (width / 2), position.getY() + (height / 2) };

    // 2. Definir la longitud de la línea del vector (e.g., 50 píxeles)
    float vectorLength = 50.0f;
    // 3. Calcular la posición final de la línea
    Vector2 endPosFor;
    endPosFor.x = positioning.x + forwardVector.x * vectorLength;
    endPosFor.y = positioning.y + forwardVector.y * vectorLength;

    // 4. Dibujar la línea (desde el centro hasta el punto final)
    // Usamos un color distintivo (e.g., GREEN) para verlo claramente.
    DrawLine(
        (int)positioning.x,
        (int)positioning.y,
        (int)endPosFor.x,
        (int)endPosFor.y,
        GREEN
    );
    //2.VECTOR RIGHT
    b2Vec2 rightvector = GetRightVector();
    Vector2 endPos;
    endPos.x = positioning.x + rightvector.x * vectorLength;
    endPos.y = positioning.y + rightvector.y * vectorLength;

    DrawLine(
        (int)positioning.x,
        (int)positioning.y,
        (int)endPos.x,
        (int)endPos.y,
        YELLOW
    );


    // Dibujar waypoints si es IA
    if (!isPlayer && !waypoints.empty())
    {
        // Dibujar todos los waypoints
        for (size_t i = 0; i < waypoints.size(); i++)
        {
            Color color = (i == currentWaypointIndex) ? RED : BLUE;
            DrawCircle(
                (int)waypoints[i].getX(),
                (int)waypoints[i].getY(),
                8.0f,
                color
            );
            DrawText(
                TextFormat("%d", i),
                (int)waypoints[i].getX() - 5,
                (int)waypoints[i].getY() - 20,
                15,
                WHITE
            );
        }

        // Dibujar línea hacia el waypoint actual
        Vector2D center = GetCenter();
        Vector2D target = GetCurrentWaypoint();
        DrawLine(
            (int)center.getX(),
            (int)center.getY(),
            (int)target.getX(),
            (int)target.getY(),
            YELLOW
        );
    }
    // **********************************************

    //dibujar centro body
    // Obtener posición del cuerpo (en metros)

    // Convertir a píxeles
    float centerX = METERS_TO_PIXELS(pos.x);
    float centerY = METERS_TO_PIXELS(pos.y);

    // Dibujar un círculo pequeño en el centro
    DrawCircle((int)centerX, (int)centerY, 5, MAGENTA);

    // También podemos dibujar un texto para debug
    DrawText(
        TextFormat("Center (%.2f, %.2f)", centerX, centerY),
        (int)centerX + 8,
        (int)centerY - 8,
        12,
        MAGENTA
    );
#pragma endregion

    return true;
}



