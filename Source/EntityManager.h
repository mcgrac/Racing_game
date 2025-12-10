#pragma once

#include "Module.h"
#include"Entity.h"
#include <list>

class Player;

class EntityManager{

public:

	EntityManager(Application* app, bool start_enabled = true);
	EntityManager();

	bool Init();
	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool Render();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(Module* listener, EntityType type, Vector2D position);
	
	void DestroyEntity(Entity* entity);
	void AddEntity(Entity* entity);

public:

	std::list<Entity*> entities;
	//std::list<std::shared_ptr<Entity>> entities;
};
