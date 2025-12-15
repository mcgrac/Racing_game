#pragma once
#include <memory>
#include "Vector2D.h"
#include "Application.h"
#include <vector>
#include "raylib.h"
#include "box2d/box2d.h"
#include "ModulePhysics.h"
#include "Module.h"
#include "Animation.h"
#include <fstream>      
#include <sstream>      
#include <string>       
#include <vector>  

enum class EntityType
{
	PLAYER,
	AI,
	TURBO_ON_ROAD,
	ROCK,
	CHECKPOINT,
};

class PhysBody;
class ModuleGame;

class Entity
{
public:

	Entity() {}
	//Entity(const Vector2D& pos);
	Entity(float x, float y);
	Entity(Module* _listener, const Vector2D& pos, EntityType _type);

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

	virtual bool Render() 
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
	inline const Vector2D& GetPosition() const { return position; }
	inline float GetX() const { return position.getX(); }
	inline float GetY() const { return position.getY(); }

	// Setters
	inline void SetPosition(const Vector2D& pos) { position = pos; }
	inline void SetPosition(float x, float y) { position.setX(x); position.setY(y); }
	inline PhysBody* GetPhysBody() { return physBody; }
	inline void SetIsPlayer(bool b) { isPlayer = b; }

protected:
	Vector2D position;
	PhysBody* physBody;
	Module* listener;

	//control
	bool isPlayer;

	//dimensions
	float width;
	float height;

	// Getters for dimensions
	inline float GetWidth() const { return width; }
	inline float GetHeight() const { return height; }
	inline float GetCenterX() const { return position.getX() + GetWidth() / 2.0f; }
	inline float GetCenterY() const { return position.getY() + GetHeight() / 2.0f; }

public:

	// centerd position
	Vector2D GetCenter() const;

	EntityType type;
	bool active = true;
	bool pendingToDelete = false;

	bool renderable = true;
};
