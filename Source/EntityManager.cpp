#include "EntityManager.h"
#include "Player.h"
#include"ModuleGame.h"

EntityManager::EntityManager(Application* app, bool start_enabled)
{
	//name = "entitymanager";
}

EntityManager::EntityManager()
{
}

bool EntityManager::Init()
{
	LOG("Creating EntityManager context");
	bool ret = true;

	return ret;
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	for (const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true;

	//Iterates over the entities and calls Start
	for (const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;

	for (const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->CleanUp();
	}

	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(Module* listener, EntityType type, Vector2D position)
{
	Entity* entity;

	//create entities
	switch (type)
	{
	case EntityType::PLAYER:
		//entity = new Player(listener, Vector2D(400.0f, 300.0f), type);
		break;

	default:
		break;
	}

	entities.push_back(entity);

	std::cout << "Entities list size: " << entities.size() << std::endl;
	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	entity->CleanUp();
	entities.remove(entity);
}

void EntityManager::AddEntity(Entity* entity)
{
	if (entity != nullptr) entities.push_back(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	//List to store entities pending deletion
	std::list<Entity*> pendingDelete;

	//Iterates over the entities and calls Update//List to store entities pending deletion
	for (const auto entity : entities)
	{
		//If the entity is marked for deletion, add it to the pendingDelete list
		if (entity->pendingToDelete)
		{
			pendingDelete.push_back(entity);
		}

		if (entity->active == false) continue;
		entity->Update(dt);
	}

	//Now iterates over the pendingDelete list and destroys the entities
	for (const auto entity : pendingDelete)
	{
		DestroyEntity(entity);
	}

	return ret;
}

bool EntityManager::Render()
{
	bool ret = true;

	for (const auto entity : entities) {

		if (entity->active == false) continue;
		ret = entity->Render();
	}
	return ret;
}
