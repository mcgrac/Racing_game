#pragma once
#include "Entity.h"

class Off_road :public Entity {
public:
	Off_road(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int* points, int size, int16 groupIndex = 0);
	~Off_road();
private:
	void Initialize(uint16 category, uint16 maskBits, int16 groupIndex, int* points, int size);
};
