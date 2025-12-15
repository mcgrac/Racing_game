#pragma once
#include "Entity.h"

class Checkpoint: public Entity {
public:

	Checkpoint(Module* _listener, const Vector2D& pos, const int* points, float _rotation, EntityType _type, int _id, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~Checkpoint();

private:

	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex);
	int id; //to identify the checkpoints
	float rotation;
};
