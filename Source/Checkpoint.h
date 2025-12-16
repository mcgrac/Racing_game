#pragma once
#include "Entity.h"

class Checkpoint: public Entity {
public:

	Checkpoint(Module* _listener, const Vector2D& pos, EntityType _type, int _id, uint16 category, uint16 maskBits, int16 groupIndex, const int* points, int size);
	~Checkpoint();

	inline int GetId() { return id; }
	inline Vector2D GetCenter() { return center; }

private:

	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex, const int* points, int size);

	int id; //to identify the checkpoints
	Vector2D center;
};
