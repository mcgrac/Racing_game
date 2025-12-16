#pragma once
#include "raylib.h"
#include "button.h"
#include "Scene.h"

class StartScreen {
private:
	enum MainMenuOption {
		START,
		HELP,
		BADGE,
		QUIT,
		MAIN_MENU_OPTION_COUNT
	};

	MainMenuOption selectedOption = START;
	bool hasPressStart = false;
	bool hasPressHelp = false;
	bool hasPressBadge = false;
	bool hasPressQuit = false;

	Texture2D backgroundMainMenu = { 0 };
	Texture2D mainMenuTitle = { 0 };

	Texture2D buttonPlay = { 0 };
	Texture2D buttonPlayPressed = { 0 };
	Texture2D buttonHelp = { 0 };
	Texture2D buttonHelpPressed = { 0 };
	Texture2D buttonBadges = { 0 };
	Texture2D buttonBadgesPressed = { 0 };
	Texture2D buttonQuit = { 0 };
	Texture2D buttonQuitPressed = { 0 };

	Texture2D buttonArrow = { 0 };

	Sound arrowMove = { 0 };
	Sound buttonSelect = { 0 };
	std::vector<Button*> buttons;
	bool buttonsLoaded = false;

	bool LoadButtons();
	void UnloadButtons();

public:
	StartScreen() {}
	~StartScreen();
	
	bool LoadBackgroundMainMenu();
	void UnloadBackgroundMainMenu();

	void Update();
	void Draw();

	bool GetHasPressStart() const { return hasPressStart; }
	bool GetHasPressHelp() const { return hasPressHelp; }
	bool GetHasPressBadge() const { return hasPressBadge; }
	bool GetHasPressQuit() const { return hasPressQuit; }
	
	void Reset() {
		hasPressStart = false;
		hasPressHelp = false;
		hasPressBadge = false;
		hasPressQuit = false;
	}
};