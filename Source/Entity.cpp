#include "Entity.h"
#include "ModuleGame.h"

//Entity::Entity() : position(0.0f, 0.0f) {}

//Entity::Entity(const Vector2D& pos) : position(pos) {}

Entity::Entity(float x, float y) : position(x, y) {}

Entity::Entity(Module* _listener, const Vector2D& pos, EntityType _type) :  
	listener(_listener),
	position(pos), 
	type(_type), 
	active(true) {

	//physBody->listener = _listener; // quién recibe BeginContact
}

Vector2D Entity::GetCenter() const {
	return Vector2D(position.getX() + GetWidth() / 2.0f,
		position.getY() + GetHeight() / 2.0f);
}
