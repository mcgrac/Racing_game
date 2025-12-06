#pragma once
#include "Player.h"
#include "Entity.h"

class Player;

class GameCamera : public Entity{

public:

	GameCamera();
	GameCamera(float viewportW, float viewportH);

	~GameCamera();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;
	
	// Configure world limits
	void SetBounds(const Vector2D& min, const Vector2D& max);
	void SetBounds(float minX, float maxX, float minY, float maxY);
	void RemoveBounds();


	void SetViewportSize(float width, float height);

	// Establish velocity
	void SetSmoothSpeed(float speed) { smoothSpeed = speed; }
	float GetSmoothSpeed() const { return smoothSpeed; }

	// Updates camera ith player position
	void FollowPlayer(Player* player);
	void SetTarget(const Vector2D& targetPos);

	void CenterOn(const Vector2D& pos);
	void CenterOn(float x, float y);

	Camera2D GetRaylibCamera() const;

	// Getters
	float GetX() const { return position.getX(); }
	float GetY() const { return position.getY(); }

	// Setters posotion camera manually
	void SetPosition(Vector2D newPos);

private:
	Vector2D target;
	float smoothSpeed; //(0.0 - 1.0)

	// Límites del mundo (opcional)
	Vector2D minBounds;
	Vector2D maxBounds;
	bool useBounds;

	float viewportWidth;
	float viewportHeight;

};
