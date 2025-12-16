#pragma once
#include "raylib.h"

class BadgesScreen
{
public:
    BadgesScreen() = default;
    ~BadgesScreen();

    bool Load();    
    void Unload();

    void Update(float dt);
    void Draw() const;

    // Achievement: Lose a race
    void TriggerLoseRaceBadge();
    bool IsShowing() const { return showing; }
    bool LoseRaceUnlocked() const { return loseRaceUnlocked; }

private:
    Texture2D quest8 = { 0 };
    Sound gameOverSfx = { 0 };

    bool assetsLoaded = false;

    bool loseRaceUnlocked = false;
    bool showing = false;
    bool playedSfx = false;

    float timer = 0.0f;
    float showDuration = 3.5f; 
};
