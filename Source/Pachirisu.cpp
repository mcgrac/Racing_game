#include "Globals.h"
#include "Pachirisu.h"

Pachirisu::Pachirisu(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex)
    : Characters(_listener, startPos, _type, category, maskBits)
{
    LoadAnimations();
    InitPhysics(category, maskBits, groupIndex);
}

Pachirisu::~Pachirisu()
{

}

void Pachirisu::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
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

void Pachirisu::SyncPositionFromPhysics()
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
bool Pachirisu::Start()
{
    LOG("Loading player");
    return true;
}

#pragma region LOADING / UNLOADING
void Pachirisu::LoadAnimations()
{
    //Idle animations
    idleAnimation.AddTexture("Assets/Textures/Cars/CarPachirisuIdle1.png");
    idleAnimation.AddTexture("Assets/Textures/Cars/CarPachirisuIdle2.png");

    preparingAttack.AddTexture("Assets/Textures/Cars/PachirisuAttack1.png");
    preparingAttack.AddTexture("Assets/Textures/Cars/PachirisuAttack2.png");
    preparingAttack.AddTexture("Assets/Textures/Cars/PachirisuAttack3.png");

    attackAnimation.AddTexture("Assets/Textures/cars/CarPachirisuAttack1.png");

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
void Pachirisu::UpdateAnims(float dt)
{
    switch (currentState)
    {
    case Characters::State::IDLE:
        idleAnimation.Update(dt);
        break;
    case Characters::State::ATTACK:
        attackAnimation.Update(dt);
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
bool Pachirisu::CleanUp()
{
    LOG("Unloading player");

    return true;
}

// Update: draw background
bool Pachirisu::Update(float dt)
{
    //std::cout << "State: " << (int)currentState << std::endl;
    if (IsKeyPressed(KEY_SPACE) && currentState == State::IDLE && isPlayer) {
        Attack();
    }

    UpdateAnims(dt); //update current animation depending on the state


    if (currentState == State::PREPARING_ATTACK) {
        if (preparingAttack.GetCurrentFrame() == 2) {

            //reset animation
            preparingAttack.Reset();

            //create discharge
            if(!attack)
            {
                attack = new AttackPachirisu(listener, position, EntityType::ATTACK, PhysicCategory::ATTACK, PhysicCategory::AI);
            }

            currentState = State::ATTACK;
        }
    }
    if (currentState == State::ATTACK) {
        //std::cout << "attacking" << std::endl;
        stateTimer += dt;

        attack->SetPosition(position);
        attack->GetPhysBody()->SetPos(
            position.getX() + (width/2),
            position.getY() + (height/2)
        );

        attack->Update(dt);

        if (stateTimer >= 3.0f) {

            delete attack;
            attack = nullptr;

            currentState = State::IDLE;
            stateTimer = 0.0f;
        }
    }

    if (currentState == State::STUNNED) {
        //be stunned for a while
        if (stateTimer >= 2.0f) {
            std::cout << "max Speed before stunned: " << GetMaxSpeed() << std::endl;
            maxForwardSpeed = maxForwardSpeed / 2.0f;
            std::cout << "max Speed after stunned" << GetMaxSpeed() << std::endl;
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
void Pachirisu::ApplyDrag()
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
void Pachirisu::ApplyLateralFriction()
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
void Pachirisu::ApplyAIControl(float dt)
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

    // 1. Objective waypoint
    Vector2D targetWaypoint = GetCurrentWaypoint();
    Vector2D currentPos = GetCenter();

    // 2. Verify if we are in the waypoint
    float distanceToWaypoint = currentPos.distanceEuclidean(targetWaypoint);

    if (distanceToWaypoint < waypointReachRadius) {
        AdvanceToNextWaypoint();
        targetWaypoint = GetCurrentWaypoint();
    }

    // 3. Calculate direction towards objective
    float steeringAngle = CalculateSteeringAngle(targetWaypoint);

    // 4. Decide if accelerate or brake
    bool shouldAccelerate = ShouldAccelerate(targetWaypoint);
    bool shouldBrake = ShouldBrake(targetWaypoint);

    // -------- ACCELERATE --------
    if (shouldAccelerate && !shouldBrake)
    {
        if (speed < maxForwardSpeed)
        {
            b2Vec2 force = (accelerationForce * 0.9f) * forwardVector;
            body->ApplyForceToCenter(force, true);
        }
    }

    // -------- BRAKE --------
    if (shouldBrake && speed > 0.1f)
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
            // steeringAngle > 0 → tuen right
            // steeringAngle < 0 → turn left

            float torqueAmount = steeringAngle * turnTorque;

            // Clamp for very closed turns
            if (torqueAmount > turnTorque) torqueAmount = turnTorque;
            if (torqueAmount < -turnTorque) torqueAmount = -turnTorque;

            body->ApplyTorque(torqueAmount, true);
        }
    }

    // -------- DRAGS --------
    ApplyDrag();
    ApplyLateralFriction();

    this->speed = speed;
}
#pragma region AUXILIARS AI
float Pachirisu::CalculateSteeringAngle(const Vector2D& targetPos)
{
    Vector2D currentPos = GetCenter();

    Vector2D toTarget = targetPos - currentPos;
    toTarget.normalized();

    b2Vec2 forward = GetForwardVector();

    // dot product: cos(θ) = A·B / (|A||B|)
    float dot = forward.x * toTarget.getX() + forward.y * toTarget.getY();

    // determine sign of the turn
    // cross_z = Ax*By - Ay*Bx
    float cross = forward.x * toTarget.getY() - forward.y * toTarget.getX();

    // 5. calcula angle with tangent
    float angle = atan2f(cross, dot);
    return angle;
}

bool Pachirisu::ShouldAccelerate(const Vector2D& targetPos)
{
    Vector2D currentPos = GetCenter();
    float distanceToTarget = currentPos.distanceEuclidean(targetPos);

    // Calcular el ángulo hacia el objetivo
    float steeringAngle = fabs(CalculateSteeringAngle(targetPos));

    // No acelerar si estamos girando mucho (curva cerrada)
    if (steeringAngle > PI / 4.0f) {  // > 45 grados
        return false;
    }

    // Acelerar si estamos lejos del objetivo
    if (distanceToTarget > 100.0f) {
        return true;
    }

    // Acelerar si vamos despacio
    if (GetSpeed() < maxForwardSpeed * 0.7f) {
        return true;
    }

    return false;
}

bool Pachirisu::ShouldBrake(const Vector2D& targetPos)
{
    Vector2D currentPos = GetCenter();
    Vector2D nextWaypoint = GetNextWaypoint();

    // Calcular el ángulo de la próxima curva
    float angleToNext = fabs(CalculateSteeringAngle(nextWaypoint));

    // Frenar si la próxima curva es cerrada y vamos rápido
    if (angleToNext > PI / 3.0f && GetSpeed() > maxForwardSpeed * 0.6f) {  // > 60 grados
        return true;
    }

    // Frenar si estamos muy cerca del waypoint
    float distanceToTarget = currentPos.distanceEuclidean(targetPos);
    if (distanceToTarget < waypointReachRadius * 1.5f) {
        return true;
    }

    return false;
}
#pragma endregion

#pragma endregion

#pragma region NON AI CONTROL
void Pachirisu::ApplyCarPhysics(float dt) {

    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();
    b2Vec2 forwardVector = GetForwardVector();

    float speed = b2Dot(currentVelocity, forwardVector);
    float absoluteSpeed = fabs(speed); 

    //-------------------------MOVING FORWARD/BACK--------------------------
    // Acceleration
    if (IsKeyDown(KEY_W))
    {
        if (speed < maxForwardSpeed)
        {
            b2Vec2 force = accelerationForce * forwardVector;
            body->ApplyForceToCenter(force, true);

        }
    }
    // Slow down
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
    //check for the boost
    Boost(dt);

    this->speed = speed + turboPower;
}
void Pachirisu::Attack()
{
    std::cout << "Pressed space key -> attack" << std::endl;
    previousState = currentState;
    currentState = State::PREPARING_ATTACK;

}
#pragma endregion

void Pachirisu::Boost(float dt)
{
    if (!isBoosted) {
        SetMaxSpeed(10.0f); //setVelocity with no boost
        return;
    }

    turboPower -= dt;
    if (turboPower <= 0.0f) {
        isBoosted = false;
        turboPower = 0.0f;
    }
}

bool Pachirisu::Render() {

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
        currentTexture = attackAnimation.GetCurrentTexture();
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
#pragma endregion

    return true;
}



