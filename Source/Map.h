#pragma once
#include "Globals.h"
#include "raylib.h"
#include "ModulePhysics.h"
#include "EntityManager.h"

enum class MapType {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3
};

class Map {

public:

    Map(MapType _mapType):mapType(_mapType){}
    virtual ~Map() {}

    //virtual void Start() = 0;     // Construye el nivel
    //virtual void Update() = 0;
    virtual void RenderBackground() = 0;  //render level each frame
    virtual void RenderTop() = 0;  //render level each frame
    virtual void CleanUp() = 0; // Libera memoria

    virtual bool Load() = 0;
   
    const std::vector<Vector2D>& GetStartingGrid() const{ return startingPositions; }
    // Number of left positions
    int GetStartingPositionsCount() const { return startingPositions.size(); }

    MapType GetMapType() const { return mapType; }

protected:
    //virtual void CreateLevel();
    MapType mapType;
    std::vector<Vector2D> startingPositions;  // Posiciones de la parrilla
};
