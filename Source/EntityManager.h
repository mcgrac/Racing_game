#pragma once

#include "Module.h"
#include"Entity.h"
#include <list>

class Player;

class EntityManager : public Module {

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
	update_status Update() override;

	bool Render();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, Vector2D position);
	
	void DestroyEntity(Entity* entity);
	void AddEntity(Entity* entity);

	Player* GetPlayer() { return player; }

public:

	std::list<Entity*> entities;
	Player* player = nullptr;
	//std::list<std::shared_ptr<Entity>> entities;
};
