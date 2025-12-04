#include "Entity.h"

//Entity::Entity() : position(0.0f, 0.0f) {}

Entity::Entity(const Vector2D& pos) : position(pos) {}

Entity::Entity(float x, float y) : position(x, y) {}