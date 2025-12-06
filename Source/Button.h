#pragma once
#include "raylib.h"

class Button {

public:
    Button(Texture2D buttonUp,
        Texture2D buttonDown,
        Texture2D buttonSelected,
        Sound pressSound, Sound selectSound,
        Vector2 position,
        float pressDuration = 0.2f);

    ~Button() {}

    void Update();
    void Draw();

    void Select();
    void Press();              // Call this from Scene
    void Release();
    bool IsPressed() const { return isPressed; }
    bool IsSelected() const { return isSelected; }



private:
    Texture2D buttonUpSprite;
    Texture2D buttonDownSprite;
    Texture2D buttonSelectedSprite;

    Sound pressSound;
    Sound selectSound;

    Vector2 position;

    bool isPressed = false;
    bool isSelected = false;
    float pressTime = 0.0f;
    float pressDuration;       // How long the button stays down
};