#include "raylib.h"
#include <vector>
#include "Scene.h"
#include "CharacterSelectionScreen.h"
#include "Button.h"

void CharacterSelectionScreen::LoadTextures() {
	SelectedChansey = LoadTexture("Assets/Textures/UI/CharacterSelect/CharacterSelectedChansey.png");
	SelectedCleffa = LoadTexture("Assets/Textures/UI/CharacterSelect/CharacterSelectedCleffa.png");
	SelectedPachirisu = LoadTexture("Assets/Textures/UI/CharacterSelect/CharacterSelectedPachirisu.png");
	SelectedMeganium = LoadTexture("Assets/Textures/UI/CharacterSelect/CharacterSelectedMeganium.png");
	Cursor = LoadTexture("Assets/Textures/UI/CharacterSelect/CursorCharacterSelection.png");

}

void CharacterSelectionScreen::LoadSounds() {
	buttonPressed = LoadSound("Assets/Sound/Sfx/ClickButtonUI.mp3");
	buttonSelected = LoadSound("Assets/Sound/Sfx/SelectButtonUI.mp3");
}

void CharacterSelectionScreen::LoadSounds() {
	LoadTextures();
	LoadSounds();

	//create the buttons
}

void CharacterSelectionScreen::Draw() {
	//draw background
	switch(charSelected) {
	case characterSelection::CHANSEY:
		DrawTexture(SelectedChansey, 0, 0, WHITE);
		break;
	case characterSelection::CLEFFA:
		DrawTexture(SelectedCleffa, 0, 0, WHITE);
		break;
	case characterSelection::PACHIRISU:
		DrawTexture(SelectedPachirisu, 0, 0, WHITE);
		break;
	case characterSelection::MEGANIUM:
		DrawTexture(SelectedMeganium, 0, 0, WHITE);
		break;
	}
	//draw cursor
	switch (charSelected) {
	case characterSelection::CHANSEY:
		DrawTexture(Cursor, 100, 50, WHITE);
		break;
	case characterSelection::CLEFFA:
		DrawTexture(Cursor, 100, 150, WHITE);
		break;
	case characterSelection::PACHIRISU:
		DrawTexture(Cursor, 100, 150, WHITE);
		break;
	case characterSelection::MEGANIUM:
		DrawTexture(Cursor, 100, 150, WHITE);
		break;
	}
}

void CharacterSelectionScreen::UnloadAssets() {
	//unload textures
	UnloadTexture(SelectedChansey);
	UnloadTexture(SelectedCleffa);
	UnloadTexture(SelectedPachirisu);
	UnloadTexture(SelectedMeganium);
	//unload sounds
	UnloadSound(buttonPressed);
	UnloadSound(buttonSelected);
}

void CharacterSelectionScreen::Update() {
	//check if arrows are pressed
	//move the cursor acordingly
	//ckeck if enter is pressed, to select the charcter
}