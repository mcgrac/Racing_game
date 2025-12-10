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

public:
	StartScreen() {}
	//void Update();
	//void Draw();

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