#pragma once
#include <memory>
#include "Vector2D.h"
#include "Globals.h"
#include<vector>
#include "raylib.h"

enum class EntityType
{
	PLAYER
};

class PhysBody;

class Entity : public std::enable_shared_from_this<Entity>
{
public:

	Entity() {}
	Entity(const Vector2D& pos);
	Entity(float x, float y);
	Entity(EntityType type) : type(type), active(true) {}

	virtual ~Entity() = default;

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Destroy()
	{
		return true;
	}

	void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {

	};

	// Getters
	const Vector2D& GetPosition() const { return position; }
	float GetX() const { return position.getX(); }
	float GetY() const { return position.getY(); }

	// Setters
	void SetPosition(const Vector2D& pos) { position = pos; }
	void SetPosition(float x, float y) { position.setX(x); position.setY(y); }

protected:
	Vector2D position;
public:

	EntityType type;
	bool active = true;
	bool pendingToDelete = false;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	bool renderable = true;
};
