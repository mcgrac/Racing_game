#pragma once
#include"Entity.h"

class Interactables : public Entity {
public:

	Interactables(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~Interactables();

	inline void SetIsDestroyed(bool b) { isDestroyed = b; }

protected:

	virtual void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex);
	virtual void LoadAnimations();

	float width;
	float height;

	bool isDestroyed;
};