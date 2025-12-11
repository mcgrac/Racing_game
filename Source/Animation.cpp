#include "Animation.h"
#include "Globals.h"

Animation::Animation(float frameDuration)
    : currentFrame(0),
    frameTime(0.0f),
    frameDuration(frameDuration)
{
}

Animation::~Animation()
{
    for (auto& tex : textures) {
        UnloadTexture(tex);
    }
    textures.clear();
}

void Animation::AddTexture(const char* path)
{
    Texture2D tex = LoadTexture(path);
    if (tex.id != 0) {
        textures.push_back(tex);
        LOG("Animation texture loaded: %s", path);
    }
    else {
        LOG("ERROR: Failed to load animation texture: %s", path);
    }
}

void Animation::Update(float dt)
{
    if (textures.empty()) return;
    frameTime += dt;

    if (frameTime >= frameDuration) {
        frameTime = 0.0f;
        currentFrame++;

        if (currentFrame >= textures.size()) {
            currentFrame = 0;  // Loop
        }
    }
}

void Animation::Reset()
{
    currentFrame = 0;
    frameTime = 0.0f;
}

Texture2D Animation::GetCurrentTexture() const
{
    if (textures.empty()) {
        return Texture2D{ 0 };
    }
    return textures[currentFrame];
}