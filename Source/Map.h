#pragma once
#include "Globals.h"
#include "raylib.h"
#include "ModulePhysics.h"
#include "EntityManager.h"

class Map {

public:

    virtual ~Map() {}

    virtual void Start() = 0;     // Construye el nivel
    virtual void Update() = 0;
    virtual void RenderBackground() = 0;  //render level each frame
    virtual void RenderTop() = 0;  //render level each frame
    virtual void CleanUp() = 0; // Libera memoria


protected:
    //virtual void CreateLevel();

private:

};
