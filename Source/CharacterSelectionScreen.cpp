#include "raylib.h"
#include <vector>
#include "CharacterSelectionScreen.h"
//#include "Button.h"

CharacterSelectionScreen::CharacterSelectionScreen(){}

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

void CharacterSelectionScreen::Start() {
	LoadTextures();
	LoadSounds();

	charSelected = characterSelection::CHANSEY;
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
	
}

void CharacterSelectionScreen::UnloadAssets() {
	//unload textures
	UnloadTexture(SelectedChansey);
	UnloadTexture(SelectedCleffa);
	UnloadTexture(SelectedPachirisu);
	UnloadTexture(SelectedMeganium);
	UnloadTexture(Cursor);
	//unload sounds
	UnloadSound(buttonPressed);
	UnloadSound(buttonSelected);
}

void CharacterSelectionScreen::Update() {
	//check if arrows are pressed
	switch (charSelected) {
	case characterSelection::CHANSEY:
		if (IsKeyPressed(KEY_RIGHT)) {
			charSelected = characterSelection::MEGANIUM;
			PlaySound(buttonSelected);
		}
		else if (IsKeyPressed(KEY_DOWN)) {
			charSelected = characterSelection::PACHIRISU;
			PlaySound(buttonSelected);
		}
		break;
	case characterSelection::CLEFFA:
		if (IsKeyPressed(KEY_UP)) {
			charSelected = characterSelection::PACHIRISU;
			PlaySound(buttonSelected);
		}
		break;
	case characterSelection::PACHIRISU:
		if (IsKeyPressed(KEY_UP)) {
			charSelected = characterSelection::CHANSEY;
			PlaySound(buttonSelected);
		}
		else if (IsKeyPressed(KEY_DOWN)) {
			charSelected = characterSelection::CLEFFA;
			PlaySound(buttonSelected);
		}
		break;
	case characterSelection::MEGANIUM:
		if (IsKeyPressed(KEY_LEFT)) {
			charSelected = characterSelection::CHANSEY;
			PlaySound(buttonSelected);
		}
		else if (IsKeyPressed(KEY_DOWN)) {
			charSelected = characterSelection::PACHIRISU;
			PlaySound(buttonSelected);
		}
		break;
	}
	
	//ckeck if enter is pressed, to select the charcter
	if (IsKeyPressed(KEY_ENTER)) {
		PlaySound(buttonPressed);
		switch (charSelected) {
		case characterSelection::CHANSEY:
			//return 2 and change scene
			break;
		case characterSelection::CLEFFA:
			//return 1 and change scene
			break;
		case characterSelection::PACHIRISU:
			//return 3 and change scene
			break;
		case characterSelection::MEGANIUM:
			//return 4 and change scene
			break;
		}
	}
}