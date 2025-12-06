#include "Globals.h"
#include "Player.h"

Player::Player(Application* app, bool start_enabled)
{
}

Player::Player()
    : Entity(0.0f, 0.0f),
    speed(200.0f),
    textureLoaded(false) {
    texture = { 0 };
}

Player::Player(float startX, float startY, const char* texturePath)
    : Entity(startX, startY),  // Llama al constructor Entity(float, float)
    speed(200.0f),
    textureLoaded(false) {
    texture = { 0 };
    LoadTexture(texturePath);
}

Player::Player(const Vector2D& startPos, const char* texturePath)
    : Entity(startPos),
    speed(200.0f),
    textureLoaded(false) {
    texture = { 0 };
    LoadTexture(texturePath);
}

Player::Player(const Vector2D& startPos)
    : Entity(startPos),
    speed(200.0f),
    textureLoaded(false) {
    texture = { 0 };
}

Player::~Player()
{
    UnloadTexture();
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
    LOG("position Player: (%f, %f)", position.getX(), position.getY());
    // Movement Logic
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        position.setX(position.getX() + speed * dt);
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        position.setX(position.getX() - speed * dt);
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        position.setY(position.getY() + speed * dt);
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        position.setY(position.getY() - speed * dt);
    }
    return true;
}

void Player::Render() {
    if (textureLoaded) {
        DrawTexture(texture,
            (int)position.getX(),
            (int)position.getY(),
            WHITE);
    }
    else {
        DrawRectangle((int)position.getX(),
            (int)position.getY(),
            32, 32, RED);
    }
}



