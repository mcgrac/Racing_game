#pragma once
#include "Map.h"

class Level1 : public Map {

public:

    Level1(ModulePhysics* _physics, Module* _listener, EntityManager* _entityManager);
    ~Level1();

    //void Start() override;
    //void Update() override;
    bool Load() override;
    void RenderBackground() override;
    void RenderTop() override;
    void CleanUp() override;
    void UpdateMusic() override;

protected:

private:

    void InitializeStartingGrid();
    void LoadColliders(const char* filePath);
    void LoadBoosts();
    void LoadAllChains(const char* filePath);

    ModulePhysics* physics;
    Module* listener; // listener for everithing
    EntityManager* entityManager; //creation entities

    //bosts
    std::vector<Boost*> boostsList;
    
    //textures
    Texture2D floor;
    Texture2D overMap;

    //music
    Sound inGameMusicBeggining;
    Sound inGameMusic;

};