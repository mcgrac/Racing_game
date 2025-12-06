#pragma once
#include "Map.h"

class Level1 : public Map {

public:

    Level1(ModulePhysics* physics, Module* _listener);
    ~Level1();

    void Start() override;
    void Update() override;
    void Render() override;
    void CleanUp() override;

protected:

private:

    Texture2D floor;
    Texture2D overMap;

};