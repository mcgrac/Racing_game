#include "BadgesScreen.h"

BadgesScreen::~BadgesScreen()
{
    Unload();
}

bool BadgesScreen::Load()
{
    
    const char* q2Path = "Assets/Textures/UI/Badges/Quest2.png";
    const char* q3Path = "Assets/Textures/UI/Badges/Quest3.png";
    const char* q4Path = "Assets/Textures/UI/Badges/Quest4.png";
    const char* q8Path = "Assets/Textures/UI/Badges/Quest8.png";
    const char* sfxPath = "Assets/Audio/GameOver.mp3";

    if (quest2.id != 0) UnloadTexture(quest2);
    if (quest3.id != 0) UnloadTexture(quest3);
    if (quest4.id != 0) UnloadTexture(quest4);
    if (quest8.id != 0) UnloadTexture(quest8);

    quest2 = LoadTexture(q2Path);
    quest3 = LoadTexture(q3Path);
    quest4 = LoadTexture(q4Path);
    quest8 = LoadTexture(q8Path);

    if (IsAudioDeviceReady())
        gameOverSfx = LoadSound(sfxPath);
    else
        gameOverSfx = { 0 };

   
    return (quest2.id != 0 || quest3.id != 0 || quest4.id != 0 || quest8.id != 0);
}

void BadgesScreen::Unload()
{
    if (quest2.id != 0) { UnloadTexture(quest2); quest2 = { 0 }; }
    if (quest3.id != 0) { UnloadTexture(quest3); quest3 = { 0 }; }
    if (quest4.id != 0) { UnloadTexture(quest4); quest4 = { 0 }; }
    if (quest8.id != 0) { UnloadTexture(quest8); quest8 = { 0 }; }

    if (gameOverSfx.frameCount > 0) { UnloadSound(gameOverSfx); gameOverSfx = { 0 }; }

    currentTex = nullptr;
    showing = false;
    timer = 0.0f;
}

void BadgesScreen::ShowBadge(Texture2D* tex, bool playSound)
{
    if (!tex || tex->id == 0) return;

    currentTex = tex;
    showing = true;
    timer = 0.0f;
    playedSfx = false;

    if (playSound && IsAudioDeviceReady() && gameOverSfx.frameCount > 0 && !playedSfx)
    {
        PlaySound(gameOverSfx);
        playedSfx = true;
    }
}

void BadgesScreen::TriggerLoseRaceBadge()
{
    ShowBadge(&quest8, true); 
}

void BadgesScreen::TriggerAccelerationFinishBadge()
{
    ShowBadge(&quest2, false); 
}

void BadgesScreen::TriggerTurboFinishBadge()
{
    ShowBadge(&quest3, false); 
}

void BadgesScreen::TriggerOffRoadFinishBadge()
{
    ShowBadge(&quest4, false);
}

void BadgesScreen::Update(float dt)
{
    if (!showing) return;

    timer += dt;
    if (timer >= showDuration)
        showing = false;
}

void BadgesScreen::Draw() const
{
    if (!showing || !currentTex || currentTex->id == 0) return;

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    float scale = 1.0f;
    float maxW = screenW * 0.8f;
    if (currentTex->width > maxW)
        scale = maxW / (float)currentTex->width;

    float drawW = currentTex->width * scale;
    float drawH = currentTex->height * scale;

    float x = screenW * 0.5f - drawW * 0.5f;
    float y = screenH * 0.5f - drawH * 0.5f;

    float t = timer / showDuration;
    float alpha = 1.0f;
    if (t < 0.15f) alpha = t / 0.15f;
    else if (t > 0.85f) alpha = (1.0f - t) / 0.15f;

    Color tint = WHITE;
    tint.a = (unsigned char)(alpha * 255.0f);

    DrawTextureEx(*currentTex, { x, y }, 0.0f, scale, tint);
}