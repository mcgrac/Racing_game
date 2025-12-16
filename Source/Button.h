#pragma once
#include "raylib.h"
#include<functional>

class Button {

public:
    Button(int targetScene, Texture2D buttonUp,
        Texture2D buttonDown,
        Texture2D buttonSelected,
        Sound pressSound, Sound selectSound,
        Vector2 position,
        float pressDuration = 0.2f);

    ~Button() {}

    void Update();
    void Draw();

    void Select();
    int press();              // Call this from Scene
    void Release();
    bool IsPressed() const { return isPressed; }
    bool IsSelected() const { return isSelected; }

    int TargetScene;



protected:
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



