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
