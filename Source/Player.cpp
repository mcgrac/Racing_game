#include "Globals.h"
#include "Player.h"

Player::Player(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex)
    : Characters(_listener, startPos, _type)
{   
    LoadAnimations();
    //create physbody
    InitPhysics(category, maskBits, groupIndex);
}

Player::~Player()
{
    UnloadTexture();
}

void Player::InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex)
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

float Player::GetRotation() const {
    if (!physBody || !physBody->body) return 0.0f;
    return physBody->body->GetAngle() * 180.0f / PI;
}

float Player::GetSpeed() const {
    if (!physBody || !physBody->body) return 0.0f;
    return physBody->body->GetLinearVelocity().Length();
}

float Player::GetSpeedKmh() const {
    return GetSpeed() * 3.6f;
}

b2Vec2 Player::GetForwardVector() const {
    float angle = physBody->body->GetAngle();

    // **IMPLEMENTACIÓN ESTÁNDAR para Angulo 0 = ARRIBA (Y negativo)**
    // Si angle = 0, sin(0) = 0, cos(0) = 1. Resultado: (0, -1) [Arriba]
    return b2Vec2(sinf(angle), -cosf(angle));
}
b2Vec2 Player::GetRightVector() const {
    b2Vec2 forward = GetForwardVector();

    // 2. Calculamos el vector perpendicular.
    // Usaremos la convención (y, -x) para obtener el vector a la derecha 
    // (una rotación de 90 grados en sentido horario desde el vector forward).

    // Si forward es (Fx, Fy), right será (Fy, -Fx)
    return b2Vec2(-forward.y, forward.x);
}

void Player::SyncPositionFromPhysics()
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
bool Player::Start()
{
    LOG("Loading player");
    return true;
}

#pragma region LOADING / UNLOADING
bool Player::LoadTexture(const char* texturePath) {
    if (textureLoaded) {
        UnloadTexture();
    }

    texture = ::LoadTexture(texturePath);
    textureLoaded = (texture.id > 0);
    return textureLoaded;
}

void Player::UnloadTexture() {
    if (textureLoaded) {
        ::UnloadTexture(texture);
        textureLoaded = false;
        texture = { 0 };
    }
}
void Player::LoadAnimations()
{
    //Idle animations
    idleAnimation.AddTexture("Assets/Textures/Cars/CarChanseyIdle1.png");
    idleAnimation.AddTexture("Assets/Textures/Cars/CarChanseyIdle2.png");

    
    // Obtain size textures
    if (idleAnimation.IsValid()) {
        Texture2D firstTex = idleAnimation.GetCurrentTexture();
        width = firstTex.width;
        height = firstTex.height;
        std::cout << "Player animations loaded (size: %dx%d)" << (int)width << ", " << (int)height << std::endl;
        textureLoaded = true;
    }
    else {
        std::cout << "Error getting size animations" <<std::endl;
        width = 64;
        height = 64;
    }
}
#pragma endregion

#pragma region ANIMATIONS
void Player::UpdateAnims(float dt)
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
    default:
        break;
    }
}
#pragma endregion

Vector2D Player::GetCenter() const {
    return Vector2D(position.getX() + GetWidth() / 2.0f,
        position.getY() + GetHeight() / 2.0f);
}

// Unload assets
bool Player::CleanUp()
{
    LOG("Unloading player");

    return true;
}

// Update: draw background
bool Player::Update(float dt)
{
    //if (!active) { return; }

    UpdateState(dt); //check if there is a temporal state, like stunned
    UpdateAnims(dt); //update current animation depending on the state

    //std::cout<<"position Player: " << position << std::endl;
    //std::cout << "position physBody (x, y): "
    //    << physBody->GetPosition().x << ", "
    //    << physBody->GetPosition().y << ", "
    //    << "Rotation: " << rotation << ", "
    //    << "Speed: " << speed << std::endl;

    //only control car if it is the player
    if(isPlayer)
    {
        ////-----------INPUT CAR PHYSICS------------------
        ApplyCarPhysics(dt);
        ////----------------------------------------------
    }
    else {
        //AI control
    }

    SyncPositionFromPhysics();
    return true;
}

#pragma region DRAGS
void Player::ApplyDrag()
{
    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();

    // Aplicamos una fuerza de arrastre proporcional a la velocidad
    // (Fuerza opuesta a la dirección del movimiento).
    // Usamos dragCoefficient para controlar la intensidad.
    // F_drag = -dragCoefficient * currentVelocity
    b2Vec2 dragForce = -dragCoefficient * currentVelocity;

    // Para evitar la trepidación a velocidad 0, solo aplicamos la fuerza si hay movimiento
    if (currentVelocity.LengthSquared() > 0.1f)
    {
        body->ApplyForceToCenter(dragForce, true);
    }
    else
    {
        // Si la velocidad es insignificante, la forzamos a 0 para detenerlo por completo
        body->SetLinearVelocity(b2Vec2(0, 0));
    }
}
void Player::ApplyLateralFriction()
{
    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();

    // Obtener la dirección perpendicular (lateral)
    b2Vec2 rightVector = GetRightVector();
    // Si GetForwardVector es (x, y), GetRightVector es (-y, x) o (y, -x).

    // Proyección de la velocidad en la dirección lateral (cuánto se está "derrapando")
    float lateralSpeed = b2Dot(currentVelocity, rightVector);

    // 2. Impulso: Calculamos la fuerza necesaria para anular esa velocidad lateral.
    // El impulso es proporcional a la masa y al coeficiente lateral.
    b2Vec2 lateralImpulse = lateralSpeed * rightVector;
    lateralImpulse *= body->GetMass() * lateralDrag;

    // 3. Aplicar: Aplicamos el impulso negativo para cancelar el derrape.
    body->ApplyLinearImpulse(-lateralImpulse, body->GetWorldCenter(), true);
}
#pragma endregion


void Player::ApplyCarPhysics(float dt) {
    b2Body* body = physBody->body;

    b2Vec2 currentVelocity = body->GetLinearVelocity();
    b2Vec2 forwardVector = GetForwardVector();

    // Proyección de la velocidad en la dirección hacia adelante
    float speed = b2Dot(currentVelocity, forwardVector);
    float absoluteSpeed = fabs(speed); // Velocidad sin dirección (magnitud)

    //-------------------------MOVING FORWARD/BACK--------------------------
    // 2. Aplicar la aceleración si se pulsa 'W'
    if (IsKeyDown(KEY_W))
    {
        // Solo aplica fuerza si no hemos alcanzado la velocidad máxima hacia adelante
        if (speed < maxForwardSpeed)
        {
            // F = m * a
            // Aplicamos una fuerza proporcional a 'accelerationForce' en la dirección hacia adelante.
            b2Vec2 force = accelerationForce * forwardVector;
            body->ApplyForceToCenter(force, true);
            
        }
    }
    // Opcional: Implementar frenado o marcha atrás aquí (e.g., con KEY_S)
    else if (IsKeyDown(KEY_S))
    {
        // Caso 1: Frenado (si vamos hacia adelante)
        if (speed > 0.1f)
        {
            // Aplicamos una fuerza grande (brakeForce) en la dirección opuesta al avance.
            b2Vec2 brakeForceVector = -brakeForce * forwardVector;
            body->ApplyForceToCenter(brakeForceVector, true);
        }
        // Caso 2: Marcha Atrás (si estamos parados o ya vamos hacia atrás)
        else if (speed > -maxBackwardSpeed)
        {
            // Aplicamos la fuerza de aceleración (reducida) en la dirección opuesta.
            b2Vec2 reverseForce = -accelerationForce * .95f * forwardVector; // 0.5f para que sea más lenta
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
            // Girar a la derecha (sentido horario, Box2D usa negativo)
            torqueAmount = +turnTorque;
        }
        else if (IsKeyDown(KEY_A))
        {
            // Girar a la izquierda (sentido antihorario, Box2D usa positivo)
            torqueAmount = -turnTorque;
        }

        // Aplicar el torque si se está girando
        if (torqueAmount != 0.0f)
        {
            // Aplicar el torque al centro de masa del cuerpo
            body->ApplyTorque(torqueAmount, true);
        }
    }
    //------------------------------------------------------------
    // 3. Aplicar arrastre/resistencia del aire (Drag)
    // Esto es crucial para que el coche se ralentice cuando no se acelera
    ApplyDrag();

    // 4. Aplicar fricción lateral
    // Esto evita el derrape infinito y mantiene el coche alineado con su movimiento
    ApplyLateralFriction();

    // Actualizar la variable de velocidad para el HUD/Game
    this->speed = speed;
}

bool Player::Render() {

    // Obtener posición y rotación del cuerpo físico
    b2Vec2 pos = physBody->body->GetPosition();
    float drawX = METERS_TO_PIXELS(pos.x);
    float drawY = METERS_TO_PIXELS(pos.y);

    // Obtener la textura actual según el estado
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
    }

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

    if (textureLoaded) {

        DrawTexturePro(currentTexture,
            sourceRect,
            destRect,
            origin,
            rotation,
            WHITE);

        //DrawTexturePro(texture,
        //    { 0,0,(float)texture.width, -(float)texture.height },
        //    { position.getX(), position.getY(), (float)texture.width, (float)texture.height },
        //    { texture.width / 2.0f, texture.height / 2.0f },
        //    rotation,
        //    WHITE);
    }
    else {
        DrawRectangle((int)position.getX(),
            (int)position.getY(),
            32, 32, RED);
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
    // **********************************************
#pragma endregion

    return true;
}



