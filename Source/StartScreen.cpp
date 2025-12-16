#pragma once

#include "raylib.h"
#include <vector>
#include "Scene.h"
#include "StartScreen.h"
#include "Button.h"

void StartScreen::LoadTextures() {
	backg = LoadTexture("Assets/Textures/UI/Start/Background.png");
	Arrow = LoadTexture("Assets/Textures/UI/Start/Arrow.png");
	Title = LoadTexture("Assets/Textures/UI/Start/FullTitle.png");
	unpressed_play = LoadTexture("Assets/Textures/UI/Start/Play.png");
	pressed_play = LoadTexture("Assets/Textures/UI/Start/PlayPressed.png");
	unpressed_quit = LoadTexture("Assets/Textures/UI/Start/Quit.png");
	pressed_quit = LoadTexture("Assets/Textures/UI/Start/QuitPressed.png");
	unpressed_help = LoadTexture("Assets/Textures/UI/Start/Help.png");
	pressed_help = LoadTexture("Assets/Textures/UI/Start/HelpPressed.png");
	unpressed_badges = LoadTexture("Assets/Textures/UI/Start/Badges.png");
	pressed_badges = LoadTexture("Assets/Textures/UI/Start/BadgesPressed.png");
}

void StartScreen::LoadSounds() {
	buttonPressed = LoadSound("Assets/Sound/Sfx/ClickButtonUI.mp3");
	buttonSelected = LoadSound("Assets/Sound/Sfx/SelectButtonUI.mp3");
}
//628 336
void StartScreen::Start() {
	//load all the textures and sounds
	LoadTextures();
	LoadSounds();
	//create buttons
	//play button
	Vector2 vec = { 628, 336 };
	Button* b = new Button(unpressed_play, pressed_play, unpressed_play, buttonPressed, buttonSelected, vec);
	buttons.push_back(b);

	//help button
	Vector2 vec = { 628, 448 };
	Button* b = new Button(unpressed_help, pressed_help, unpressed_help, buttonPressed, buttonSelected, vec);
	buttons.push_back(b);

	//badges button
	Vector2 vec = { 628, 560 };
	Button* b = new Button(unpressed_badges, pressed_badges, unpressed_badges, buttonPressed, buttonSelected, vec);
	buttons.push_back(b);

	//badges button
	Vector2 vec = { 628, 672 };
	Button* b = new Button(unpressed_badges, pressed_badges, unpressed_badges, buttonPressed, buttonSelected, vec);
	buttons.push_back(b);

	selButton = StartMenuSelectedButton::PLAY;
	buttonId = 0;
}

void StartScreen::Draw() {
	//draw background
	DrawTexture(backg, 0, 0, WHITE);
	//draw Title 
	DrawTexture(Title, 458, 64, WHITE);
	//draw buttons
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->Draw();
	}

	//draw the arrow in the correct place
	switch (selButton) {
	case StartMenuSelectedButton::PLAY:
		DrawTexture(Arrow, 578, 336, WHITE);
		break;
	case StartMenuSelectedButton::HELP:
		DrawTexture(Arrow, 578, 448, WHITE);
		break;
	case StartMenuSelectedButton::BADGES:
		DrawTexture(Arrow, 578, 560, WHITE);
		break;
	case StartMenuSelectedButton::QUIT:
		DrawTexture(Arrow, 578, 672, WHITE);
		break;
	}
}

void StartScreen::UnloadAssets() {
	//textures
	UnloadTexture(backg);
	UnloadTexture(Title);
	UnloadTexture(Arrow);
	UnloadTexture(unpressed_play);
	UnloadTexture(pressed_play);
	UnloadTexture(unpressed_help);
	UnloadTexture(pressed_help);
	UnloadTexture(unpressed_badges);
	UnloadTexture(pressed_badges);
	UnloadTexture(unpressed_quit);
	UnloadTexture(pressed_quit);
	//sounds
	UnloadSound(buttonPressed);
	UnloadSound(buttonSelected);
}

void StartScreen::Update() {
	IsKeyDown(KEY_UP) {
		if (ButtonId > 0) {
			ButtonsId++;
			playSound(buttonSelected);
		}
	}
	IsKeyDown(KEY_DOWN) {
		if (ButtonId < buttons.size() - 1) {//mes petit que 3
			buttonsId--;
			playSound(buttonSelected);
		}
	}
	IsKeyEnter(ENTER) {
		buttons[buttonsId]->pressed();
	}
}



