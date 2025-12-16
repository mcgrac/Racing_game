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

    
    void TriggerLoseRaceBadge(); 

    
    void TriggerAccelerationFinishBadge(); 
    void TriggerTurboFinishBadge();        
    void TriggerOffRoadFinishBadge();      

    bool IsShowing() const { return showing; }

private:
    void ShowBadge(Texture2D* tex, bool playSound);

private:
    Texture2D quest2 = { 0 }; // Acceleration
    Texture2D quest3 = { 0 }; // Turbo
    Texture2D quest4 = { 0 }; // Off-road
    Texture2D quest8 = { 0 }; // Lose race

    Sound gameOverSfx = { 0 };

    Texture2D* currentTex = nullptr;

    bool showing = false;
    bool playedSfx = false;

    float timer = 0.0f;
    float showDuration = 3.5f;
};
