#pragma once

#include "Module.h"
#include "Globals.h"
#include "box2d\box2d.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum class ColliderType {
    UNKNOWN,
    PLAYER
    // ..
};

class Entity;

class PhysBody
{
public:
	PhysBody() : body(NULL)
	{}

	void GetPhysicPosition(int& x, int& y) const;
    void SetPos(float xPixels, float yPixels);
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	// Obtener posición en píxeles
	b2Vec2 GetPosition() const;
	float GetPositionX() const { return GetPosition().x; }
	float GetPositionY() const { return GetPosition().y; }

	// Obtener acceso al cuerpo Box2D
	b2Body* GetB2Body() const;

	// Aplicar impulso lineal al centro
	void ApplyLinearImpulseToCenter(b2Vec2 impulse, bool wake = true);
	void SetLinearVelocity(const b2Vec2& vel);

public:

	Module* listener = nullptr;
	ColliderType ctype = ColliderType::UNKNOWN;
	int width, height;
	b2Body* body;
	Entity* entity = nullptr;
};

enum PhysicCategory {

	//tangible objects
	DEFAULT = 1 << 0,
	CARS = 1 << 1,
	WALLS = 1 << 2,
	SENSORS = 1 << 3,
	DESTRUCTIBLE = 1 << 4,
	AI = 1 << 5,
	CHECKPOINTS = 1 << 6,
	ATTACK = 1 << 7

};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type);
	PhysBody* CreateCircleSensor(int x, int y, int radius, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float rotation, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex);
	PhysBody* CreateChain(int x, int y, const int* points, int size, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex);
	PhysBody* CreateChainSensor(int x, int y, const int* points, int size, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex);
	PhysBody* CreatePolygon(int x, int y, int* points, int count, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex);

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	void DestroyPhysBody(PhysBody* pbody); //called in the cleanUp
	void SetCameraDebug(Camera2D camera) { cam = camera; }

	void DrawDebug();
	void UseMouseJoint(Camera2D camera);

private:

	b2World* world = nullptr;
	b2Vec2 gravity = { 0,9.81f };
	bool debug;

	//mouse joint
	b2Body* mouseGround = nullptr;
	b2MouseJoint* mouseJoint = nullptr;
	PhysBody* grabbedBody = nullptr;
	
	Camera2D cam;
};