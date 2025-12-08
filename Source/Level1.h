#pragma once
#include "Map.h"

class Level1 : public Map {

public:

    Level1(ModulePhysics* _physics, Module* _listener, EntityManager* _entityManager);
    ~Level1();

    void Start() override;
    void Update() override;
    void RenderBackground() override;
    void RenderTop() override;
    void CleanUp() override;

protected:

private:

    ModulePhysics* physics;
    Module* listener; // listener for everithing
    EntityManager* entityManager; //creation entities

    Texture2D floor;
    Texture2D overMap;

};