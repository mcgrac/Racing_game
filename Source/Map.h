#pragma once
#include "Globals.h"
#include "raylib.h"
#include "ModulePhysics.h"
#include "EntityManager.h"
#include "Boost.h"
#include "Checkpoint.h"
#include "Off_road.h"
#include <fstream>      
#include <sstream>      
#include <string>       
#include <vector>  

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
    virtual void UpdateMusic() = 0; //play music level in a loop

    virtual bool Load() = 0;
   
    const std::vector<Vector2D>& GetStartingGrid() const{ return startingPositions; }
    // Number of left positions
    int GetStartingPositionsCount() const { return startingPositions.size(); }

    MapType GetMapType() const { return mapType; }

    //characetsr control
    inline void AddChosenCharacter(int i) { pokemonsTaken.emplace_back(i); }

    inline std::vector<int> GetPokemonsTakenList() { return pokemonsTaken; }
    inline std::vector<Checkpoint*> GetCheckpointsList() { return checkpointsList; }
    inline std::vector<Boost*> GetBoostList() { return boostsList; }

protected:
    //virtual void CreateLevel();
    MapType mapType;
    std::vector<Vector2D> startingPositions;  // Posiciones de la parrilla

    //checkpoints
    std::vector<Checkpoint*> checkpointsList;

    //bosts
    std::vector<Boost*> boostsList;

    //grass or no_road zones
    std::vector<Off_road*> offRoadList;

private:

    //list of the types of pokemons already created/choosed by the player
    std::vector<int> pokemonsTaken;
    //1->Cleffa
    //2->Chansey
    //3->Pachirisu
    //4->Meganium
};