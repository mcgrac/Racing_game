#pragma once
#include "Characters.h"

class Chansey : public Characters
{
public:
	//Player();
	//Player(float startX, float startY, const char* texturePath);
	//Player(const Vector2D& startPos, const char* texturePath);
	Chansey(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);

	virtual ~Chansey();

	bool Start() override;
	bool Update(float dt) override;
	bool Render() override;
	bool CleanUp() override;
private:

	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex);

	//ANIMATIONS/TEXTURES
	void LoadAnimations();
	void UpdateAnims(float dt) override;
	bool IsTextureLoaded() const { return textureLoaded; }
	Texture texture;

	//PHYSICS
	void ApplyDrag();
	void ApplyLateralFriction();
	void SyncPositionFromPhysics();
	void Boost(float dt) override;

	//player control
	void ApplyCarPhysics(float dt);

	//AI controls
	//Control IA
	void ApplyAIControl(float dt);
	float CalculateSteeringAngle(const Vector2D& targetPos);
	bool ShouldAccelerate(const Vector2D& targetPos);
	bool ShouldBrake(const Vector2D& targetPos);
};