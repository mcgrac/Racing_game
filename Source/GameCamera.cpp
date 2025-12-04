#include "GameCamera.h"
#include <algorithm>

GameCamera::GameCamera()
    : Entity(0.0f, 0.0f),
    target(0.0f, 0.0f),
    smoothSpeed(0.1f),
    useBounds(false),
    minBounds(0.0f, 0.0f),
    maxBounds(0.0f, 0.0f),
    viewportWidth(800.0f),
    viewportHeight(600.0f) {
}

GameCamera::GameCamera(float viewportW, float viewportH)
    : Entity(0.0f, 0.0f),
    target(0.0f, 0.0f),
    smoothSpeed(0.1f),
    useBounds(false),
    minBounds(0.0f, 0.0f),
    maxBounds(0.0f, 0.0f),
    viewportWidth(viewportW),
    viewportHeight(viewportH) {
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	return true;
}

bool GameCamera::Update(float dt)
{

    // Interpolación suave hacia el objetivo
    float newX = position.getX() + (target.getX() - position.getX()) * smoothSpeed;
    float newY = position.getY() + (target.getY() - position.getY()) * smoothSpeed;

    LOG("newx: %f newy: %f", newX, newY);

    Vector2D newPos{ newX, newY };
    SetPosition(newPos);

    // Aplicar límites si están configurados
    if (useBounds) {
        float clampedX = std::max(minBounds.getX(),
            std::min(position.getX(), maxBounds.getX()));
        float clampedY = std::max(minBounds.getY(),
            std::min(position.getY(), maxBounds.getY()));
        position.setX(clampedX);
        position.setY(clampedY);
    }

	return true;
}

#pragma region BOUNDS
void GameCamera::SetBounds(const Vector2D& min, const Vector2D& max) {
    minBounds = min;
    maxBounds = max;
    useBounds = true;
}

void GameCamera::SetBounds(float minX, float maxX, float minY, float maxY) {
    minBounds = Vector2D(minX, minY);
    maxBounds = Vector2D(maxX, maxY);
    useBounds = true;
}

void GameCamera::RemoveBounds() {
    useBounds = false;
}
#pragma endregion

void GameCamera::SetViewportSize(float width, float height) {
    viewportWidth = width;
    viewportHeight = height;
}

void GameCamera::FollowPlayer(Player* player) {
    if (!player) return;

    LOG("Establishing center");
    // Establecer el objetivo en el centro del jugador
    Vector2D playerCenter = player->GetCenter();
    target = playerCenter;
}

void GameCamera::SetTarget(const Vector2D& targetPos) {
    target = targetPos;
}

void GameCamera::CenterOn(const Vector2D& pos) {
    position = pos;
    target = pos;
}

void GameCamera::CenterOn(float x, float y) {
    CenterOn(Vector2D(x, y));
}

Camera2D GameCamera::GetRaylibCamera() const {
    Camera2D cam = { 0 };
    cam.offset = { viewportWidth / 2.0f, viewportHeight / 2.0f };
    cam.target = { position.getX(), position.getY() };
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
    return cam;
}

void GameCamera::SetPosition(Vector2D newPos)
{
    position = newPos;
}

bool GameCamera::CleanUp()
{
	return true;
}
