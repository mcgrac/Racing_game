#pragma once
#include "Entity.h"


class ChanseyAttack : public Entity {
public:

	enum class EggState {
		FLYING,      
		BROKEN,      
		DISAPPEARED 
	};

	ChanseyAttack(Module* _listener, const Vector2D& pos, float carRotation, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~ChanseyAttack();

	bool Update(float dt) override;
	bool Render() override;
	bool CleanUp() override;

	EggState GetState() const { return currentState; }
	bool ShouldBeDeleted() const { return currentState == EggState::DISAPPEARED; }

private:
	void LoadAnimations();
	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex);

	void ThrowEgg(float carRotation);
	void BreakEgg();

	Animation egg;
	Animation brokenEgg;

	// Estado del huevo
	EggState currentState = EggState::FLYING;

	float flyingTimer = 0.0f;
	float brokenTimer = 0.0f;

	const float maxFlyingTime = 3.0f;   // max time flying
	const float brokenDuration = 2.0f;  // max time broken until it disappears

	const float throwForce = .5f;     // Throwing force
};

