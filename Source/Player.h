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
	Player(Module* _listener, const Vector2D& startPos, EntityType _type);

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
	float GetRotation() const;
	float GetSpeed() const;
	float GetSpeedKmh() const;

	// texture getters
	const Texture2D& GetTexture() const { return texture; }
#pragma endregion
#pragma region SETTERS
	void SetMaxSpeed(float speed) { maxForwardSpeed = speed; }
	void SetAcceleration(float accel) { accelerationForce = accel; }
	void SetTurnSpeed(float turn) { turnTorque = turn; }
#pragma endregion

	void InitPhysics();

	bool IsTextureLoaded() const { return textureLoaded; }
	void ApplyDrag();
	void ApplyLateralFriction();
	void ApplyCarPhysics(float dt);

	void SyncPositionFromPhysics();

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
	float maxForwardSpeed;      // Velocidad máxima hacia adelante (m/s)
	float maxBackwardSpeed;     // Velocidad máxima marcha atrás (m/s)
	float accelerationForce;    // Fuerza de aceleración
	float brakeForce;           // Fuerza de frenado
	float turnTorque;           // Torque de giro (rota el physBody)
	float dragCoefficient;      // Resistencia del aire
	float lateralDrag;          // Fricción lateral (anti-drift)
	float minSpeedToTurn;       // Velocidad mínima para poder girar
	float rotation;

	// Helpers
	b2Vec2 GetForwardVector() const;
	b2Vec2 GetRightVector() const;
};