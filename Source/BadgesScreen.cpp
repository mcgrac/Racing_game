#include "BadgesScreen.h"

BadgesScreen::~BadgesScreen()
{
    Unload();
}

bool BadgesScreen::Load()
{
   
    const char* badgePath = "Assets/Textures/UI/Badges/Quest8.png";
    const char* sfxPath = "Assets/Audio/Sfx/GameOver.mp3";

    if (quest8.id != 0) UnloadTexture(quest8);
    quest8 = LoadTexture(badgePath);

   
    if (IsAudioDeviceReady())
    {
        gameOverSfx = LoadSound(sfxPath);
    }
    else
    {
        gameOverSfx = { 0 };
    }

    assetsLoaded = (quest8.id != 0);
    return assetsLoaded;
}

void BadgesScreen::Unload()
{
    if (quest8.id != 0)
    {
        UnloadTexture(quest8);
        quest8 = { 0 };
    }

    if (gameOverSfx.frameCount > 0)
    {
        UnloadSound(gameOverSfx);
        gameOverSfx = { 0 };
    }

    assetsLoaded = false;
}

void BadgesScreen::TriggerLoseRaceBadge()
{
   
    loseRaceUnlocked = true;

    
    showing = true;
    timer = 0.0f;
    playedSfx = false;

    
    if (IsAudioDeviceReady() && gameOverSfx.frameCount > 0 && !playedSfx)
    {
        PlaySound(gameOverSfx);
        playedSfx = true;
    }
}

void BadgesScreen::Update(float dt)
{
    if (!showing) return;

    timer += dt;
    if (timer >= showDuration)
    {
        showing = false;
    }
}

void BadgesScreen::Draw() const
{
    if (!showing || quest8.id == 0) return;

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

  
    float scale = 1.0f;

    
    float maxW = screenW * 0.8f;
    if (quest8.width > maxW)
        scale = maxW / (float)quest8.width;

    float drawW = quest8.width * scale;
    float drawH = quest8.height * scale;

    float x = screenW * 0.5f - drawW * 0.5f;
    float y = screenH * 0.5f - drawH * 0.5f;

    
    float t = timer / showDuration;
    float alpha = 1.0f;
    if (t < 0.15f) alpha = t / 0.15f;                
    else if (t > 0.85f) alpha = (1.0f - t) / 0.15f;   

    Color tint = WHITE;
    tint.a = (unsigned char)(alpha * 255.0f);

    DrawTextureEx(quest8, { x, y }, 0.0f, scale, tint);
}