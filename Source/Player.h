#pragma once
#include "Globals.h"
#include "p2Point.h"
#include "Application.h"
#include "Entity.h"

#include"raylib.h"

class Player : public Entity
{
public:
	//Player();
	//Player(float startX, float startY, const char* texturePath);
	//Player(const Vector2D& startPos, const char* texturePath);
	Player(const Vector2D& startPos, EntityType _type);

	virtual ~Player();

	bool Start() override;
	bool Update(float dt) override;
	bool Render() override;
	bool CleanUp() override;

#pragma region GETTERS
	// Getters for dimensions
	float GetWidth() const { return texture.width; }
	float GetHeight() const { return texture.height; }
	// centerd position
	Vector2D GetCenter() const;
	float GetCenterX() const { return position.getX() + GetWidth() / 2.0f; }
	float GetCenterY() const { return position.getY() + GetHeight() / 2.0f; }
	//others
	float GetRotation() const { return rotation; }
	float GetVelocity() const { return velocity; }
	// texture getters
	const Texture2D& GetTexture() const { return texture; }
#pragma endregion
#pragma region SETTERS
	void SetMaxSpeed(float speed) { maxSpeed = speed; }
	void SetAcceleration(float accel) { acceleration = accel; }
	void SetTurnSpeed(float turn) { turnSpeed = turn; }
#pragma endregion

	bool IsTextureLoaded() const { return textureLoaded; }

	bool LoadTexture(const char* texturePath);
	void UnloadTexture();

private:

	float speed;
	Texture texture;
	bool textureLoaded;

	// Car dimensions
	float width;
	float height;

	//Car physics variables
	float rotation;          // Rotation angle in º
	float velocity;          // Current velocity
	float maxSpeed;          // maximum velocity
	float acceleration;      // Acceleration
	float deceleration;      // Natural desacceleration (friction)
	float brakeForce;        // breakeForce
	float turnSpeed;         // Turning velocity (º/second)
	float minTurnSpeed;      // minimum velocity to be able to turn
};