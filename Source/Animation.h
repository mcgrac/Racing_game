#pragma once
#include "raylib.h"
#include <vector>

class Animation
{
public:
    Animation(float frameDuration = 0.1f);
    ~Animation();

    void AddTexture(const char* path);
    void Update(float dt);
    void Reset();

    Texture2D GetCurrentTexture() const;
    bool IsValid() const { return !textures.empty(); }
    int GetCurrentFrame() const { return currentFrame; }
    int GetFrameCount() const { return textures.size(); }

private:
    std::vector<Texture2D> textures;
    int currentFrame;
    float frameTime;
    float frameDuration;
};