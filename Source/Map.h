#pragma once
#include "Globals.h"
#include "raylib.h"
#include "ModulePhysics.h"

class Map {

public:

    virtual ~Map() {}

    virtual void Start() = 0;     // Construye el nivel
    virtual void Update() = 0;      // Opcional (animaciones)
    virtual void CleanUp() = 0;   // Libera memoria


protected:
	//virtual void CreateLevel();

private:

};
