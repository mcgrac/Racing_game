#include "Button.h"

Button::Button(Texture2D buttonUp,
    Texture2D buttonDown,
    Texture2D buttonSelected,
    Sound pressSound,
    Sound selectSound,
    Vector2 position,
    float pressDuration)
    : buttonUpSprite(buttonUp),
    buttonDownSprite(buttonDown),
    buttonSelectedSprite(buttonSelected),
    pressSound(pressSound),
    selectSound(selectSound),
    position(position),
    pressDuration(pressDuration)
{
}

void Button::Select()
{
    if (!isSelected)
    {
        isSelected = true;
        PlaySound(selectSound);
    }
}

void Button::Press()
{
    if (!isPressed)
    {
        isPressed = true;
        pressTime = 0.0f;
        PlaySound(pressSound);
    }
}

void Button::Release()
{
    isPressed = false;
}

void Button::Update()
{
    if (isPressed)
    {
        // accumulate time
        pressTime += GetFrameTime();

        // auto-release when enough time passes
        if (pressTime >= pressDuration)
        {
            Release();
        }
    }
}

void Button::Draw()
{
    if (isPressed)
        DrawTexture(buttonDownSprite, (int)position.x, (int)position.y, WHITE);
    else if(isSelected) {
        DrawTexture(buttonSelectedSprite, (int)position.x, (int)position.y, WHITE);
    }
    else
        DrawTexture(buttonUpSprite, (int)position.x, (int)position.y, WHITE);
}
