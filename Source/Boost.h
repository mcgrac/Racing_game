#pragma once

#include "Interactables.h"

class Boost : public Interactables {
public:
	Boost(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~Boost();

	bool Update(float dt);
	bool Render() override;
private:

	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex) override;
	void LoadAnimations() override;

	Animation anim;
};
