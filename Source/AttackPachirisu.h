#pragma once
#include "Entity.h"


class AttackPachirisu: public Entity {
public:

	AttackPachirisu(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~AttackPachirisu();
	
	bool Update(float dt) override;
	bool Render() override;
	bool CleanUp() override;
private:
	void LoadAnimations();
	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex);

	Animation discharge;
};
