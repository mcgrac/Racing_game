#pragma once

#include "raylib.h"
#include <vector>
#include <string>
#include "Scene.h"
#include "StartScreen.h"

static const float MENU_VERTICAL_SHIFT = 80.0f;

StartScreen::~StartScreen()
{
	UnloadButtons();
	UnloadBackgroundMainMenu();
}

bool StartScreen::LoadBackgroundMainMenu()
{
	// Unload previous if any
	if (backgroundMainMenu.id != 0) UnloadTexture(backgroundMainMenu);
	if (mainMenuTitle.id != 0) UnloadTexture(mainMenuTitle);

	//textures load
	backgroundMainMenu = LoadTexture("Assets/Textures/UI/Start/Background.png");
	mainMenuTitle = LoadTexture("Assets/Textures/UI/Start/FullTitle.png");

	if (!LoadButtons()) {
		//LOG("StartScreen::LoadButtons -> some button textures failed to load. Falling back to text menu.");
	}

	return true;
}

void StartScreen::UnloadBackgroundMainMenu()
{
	if (backgroundMainMenu.id != 0) {
		UnloadTexture(backgroundMainMenu);
		backgroundMainMenu = { 0 };
	}
	if (mainMenuTitle.id != 0) {
		UnloadTexture(mainMenuTitle);
		mainMenuTitle = { 0 };
	}
}

bool StartScreen::LoadButtons() {
	// Unload previous if any
	if (buttonPlay.id != 0) UnloadTexture(buttonPlay);
	if (buttonPlayPressed.id != 0) UnloadTexture(buttonPlayPressed);
	if (buttonHelp.id != 0) UnloadTexture(buttonHelp);
	if (buttonHelpPressed.id != 0) UnloadTexture(buttonHelpPressed);
	if (buttonBadges.id != 0) UnloadTexture(buttonBadges);
	if (buttonBadgesPressed.id != 0) UnloadTexture(buttonBadgesPressed);
	if (buttonQuit.id != 0) UnloadTexture(buttonQuit);
	if (buttonQuitPressed.id != 0) UnloadTexture(buttonQuitPressed);
	if (buttonArrow.id != 0) UnloadTexture(buttonArrow);

	//textures load
	buttonPlay = LoadTexture("Assets/Textures/UI/Start/Play.png");
	buttonPlayPressed = LoadTexture("Assets/Textures/UI/Start/PlayPressed.png");
	buttonHelp = LoadTexture("Assets/Textures/UI/Start/Help.png");
	buttonHelpPressed = LoadTexture("Assets/Textures/UI/Start/HelpPressed.png");
	buttonBadges = LoadTexture("Assets/Textures/UI/Start/Badges.png");
	buttonBadgesPressed = LoadTexture("Assets/Textures/UI/Start/BadgesPressed.png");
	buttonQuit = LoadTexture("Assets/Textures/UI/Start/Quit.png");
	buttonQuitPressed = LoadTexture("Assets/Textures/UI/Start/QuitPressed.png");
	buttonArrow = LoadTexture("Assets/Textures/UI/Start/Arrow.png");

	return true;
}

void StartScreen::UnloadButtons() {
	if (buttonPlay.id != 0) { UnloadTexture(buttonPlay); buttonPlay = { 0 }; }
	if (buttonPlayPressed.id != 0) { UnloadTexture(buttonPlayPressed); buttonPlayPressed = { 0 }; }

	if (buttonHelp.id != 0) { UnloadTexture(buttonHelp); buttonHelp = { 0 }; }
	if (buttonHelpPressed.id != 0) { UnloadTexture(buttonHelpPressed); buttonHelpPressed = { 0 }; }

	if (buttonBadges.id != 0) { UnloadTexture(buttonBadges); buttonBadges = { 0 }; }
	if (buttonBadgesPressed.id != 0) { UnloadTexture(buttonBadgesPressed); buttonBadgesPressed = { 0 }; }

	if (buttonQuit.id != 0) { UnloadTexture(buttonQuit); buttonQuit = { 0 }; }
	if (buttonQuitPressed.id != 0) { UnloadTexture(buttonQuitPressed); buttonQuitPressed = { 0 }; }

	if (buttonArrow.id != 0) {
		UnloadTexture(buttonArrow); buttonArrow = { 0 }; }
}

static void ComputeButtonLayout(int screenW, int screenH,
	Texture2D btnUp[4],
	float outPosX[4], float outPosY[4],
	float outW[4], float outH[4],
	float& outTotalHeight, float& outStartY, float titleBottom)
{
	const int optionCount = 4;
	const float spacing = 12.0f;
	const float maxButtonWidth = screenW * 0.6f;
	const float topMargin = 8.0f;
	const float bottomMargin = 12.0f;

	// Encuentra el ancho máximo de las texturas disponibles
	float maxTexW = 0.0f;
	for (int i = 0; i < optionCount; ++i) {
		if (btnUp[i].id != 0 && (float)btnUp[i].width > maxTexW) maxTexW = (float)btnUp[i].width;
	}

	// Escala inicial basada en el ancho máximo permitido
	float uniformScale = 1.0f;
	if (maxTexW > 0.0f && maxTexW > maxButtonWidth) uniformScale = maxButtonWidth / maxTexW;

	// Calcular dimensiones preliminares y altura total
	outTotalHeight = 0.0f;
	for (int i = 0; i < optionCount; ++i) {
		if (btnUp[i].id != 0) {
			outW[i] = btnUp[i].width * uniformScale;
			outH[i] = btnUp[i].height * uniformScale;
		}
		else {
			// Si falta alguna textura (no debería pasar en configuración texture-only),
			// asignamos un tamaño conservador
			outW[i] = maxButtonWidth;
			outH[i] = 48.0f;
		}
		outTotalHeight += outH[i];
	}
	outTotalHeight += spacing * (optionCount - 1);

	// Comprobar si la altura total cabe en el espacio disponible debajo del título.
	float availableHeight = (float)screenH - titleBottom - bottomMargin;
	if (availableHeight < 0.0f) availableHeight = 0.0f;

	if (outTotalHeight > availableHeight && availableHeight > 1.0f) {
		// Reducir uniformScale para que la lista de botones quepa verticalmente
		float fitScale = availableHeight / outTotalHeight;
		uniformScale *= fitScale;

		// Recalcular anchuras / alturas con la nueva escala
		outTotalHeight = 0.0f;
		for (int i = 0; i < optionCount; ++i) {
			if (btnUp[i].id != 0) {
				outW[i] = btnUp[i].width * uniformScale;
				outH[i] = btnUp[i].height * uniformScale;
			}
			else {
				outW[i] = maxButtonWidth * fitScale;
				outH[i] = 48.0f * fitScale;
			}
			outTotalHeight += outH[i];
		}
		outTotalHeight += spacing * (optionCount - 1);
	}

	// Calcular startY: preferentemente centrado, pero garantizando que quede por debajo del título
	float centerStart = (float)screenH / 2.0f - outTotalHeight / 2.0f;
	outStartY = titleBottom + topMargin;
	// sólo usar el centrado si cabe sin tocar bottomMargin
	if (centerStart > outStartY && (centerStart + outTotalHeight) <= (float)screenH - bottomMargin) {
		outStartY = centerStart;
	}

	// Posiciones X/Y acumulativas
	float curY = outStartY;
	for (int i = 0; i < optionCount; ++i) {
		outPosX[i] = (float)screenW * 0.5f - outW[i] * 0.5f;
		outPosY[i] = curY;
		curY += outH[i] + spacing;
	}
}

// Update uses same layout math so hit testing matches draw
void StartScreen::Update() {
	// Keyboard/gamepad navigation remains
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
		int idx = static_cast<int>(selectedOption);
		idx = (idx - 1 + MAIN_MENU_OPTION_COUNT) % MAIN_MENU_OPTION_COUNT;
		selectedOption = static_cast<MainMenuOption>(idx);
	}

	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
		int idx = static_cast<int>(selectedOption);
		idx = (idx + 1) % MAIN_MENU_OPTION_COUNT;
		selectedOption = static_cast<MainMenuOption>(idx);
	}

	// Confirm via keyboard/gamepad
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
		switch (selectedOption) {
		case START: hasPressStart = true; break;
		case HELP:  hasPressHelp = true; break;
		case BADGE: hasPressBadge = true; break;
		case QUIT:  hasPressQuit = true; break;
		default: break;
		}
	}

	// Mouse clicks: only if button textures are loaded (no fallback)
	Texture2D btnUp[4] = { buttonPlay, buttonHelp, buttonBadges, buttonQuit };
	if (btnUp[0].id == 0 || btnUp[1].id == 0 || btnUp[2].id == 0 || btnUp[3].id == 0) {
		// textured menu not available: ignore mouse button hit-testing (no fallback as requested)
		return;
	}

	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	// compute title bottom to place buttons below it, applying vertical menu shift
	float titleBottom = screenH * 0.12f;
	if (mainMenuTitle.id != 0) {
		float maxTitleWidth = screenW * 0.8f;
		float titleScale = 1.0f;
		if (mainMenuTitle.width > 0 && mainMenuTitle.width > maxTitleWidth) titleScale = maxTitleWidth / (float)mainMenuTitle.width;
		float drawH = mainMenuTitle.height * titleScale;
		float drawY = screenH / 8.0f + MENU_VERTICAL_SHIFT;
		titleBottom = drawY + drawH + 12.0f;
	}
	else {
		titleBottom = screenH * 0.12f + 12.0f + MENU_VERTICAL_SHIFT;
	}

	float posX[4], posY[4], w[4], h[4], totalH, startY;
	ComputeButtonLayout(screenW, screenH, btnUp, posX, posY, w, h, totalH, startY, titleBottom);

	Vector2 mouse = GetMousePosition();
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		for (int i = 0; i < 4; ++i) {
			Rectangle r = { posX[i], posY[i], w[i], h[i] };
			if (CheckCollisionPointRec(mouse, r)) {
				selectedOption = static_cast<MainMenuOption>(i);
				switch (selectedOption) {
				case START: hasPressStart = true; break;
				case HELP:  hasPressHelp = true; break;
				case BADGE: hasPressBadge = true; break;
				case QUIT:  hasPressQuit = true; break;
				default: break;
				}
			}
		}
	}
}

void StartScreen::Draw() {
	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	// Draw background (must be present)
	if (backgroundMainMenu.id != 0) {
		Rectangle src = { 0.0f, 0.0f, (float)backgroundMainMenu.width, (float)backgroundMainMenu.height };
		Rectangle dst = { 0.0f, 0.0f, (float)screenW, (float)screenH };
		Vector2 origin = { 0.0f, 0.0f };
		DrawTexturePro(backgroundMainMenu, src, dst, origin, 0.0f, WHITE);
	}
	else {
		// nothing to draw if background missing (no fallbacks)
		return;
	}

	// Draw title texture (must be present)
	float titleBottom = screenH * 0.12f;
	if (mainMenuTitle.id != 0) {
		float maxTitleWidth = screenW * 0.8f;
		float titleScale = 1.0f;
		if (mainMenuTitle.width > 0 && mainMenuTitle.width > maxTitleWidth) titleScale = maxTitleWidth / (float)mainMenuTitle.width;

		float drawW = mainMenuTitle.width * titleScale;
		float drawH = mainMenuTitle.height * titleScale;

		int drawX = screenW / 2 - (int)(drawW / 2);
		int drawY = screenH / 8; // position above center

		Vector2 titlePos;
		titlePos.x = (float)drawX;
		titlePos.y = (float)drawY;
		DrawTextureEx(mainMenuTitle, titlePos, 0.0f, titleScale, WHITE);

		titleBottom = (float)drawY + drawH + 12.0f;
	}
	else {
		// no fallback text: if title missing, we still proceed to draw buttons below default titleBottom
		titleBottom = screenH * 0.12f + 12.0f;
	}

	// Only draw textured buttons (no fallbacks)
	Texture2D btnUp[4] = { buttonPlay, buttonHelp, buttonBadges, buttonQuit };
	Texture2D btnDown[4] = { buttonPlayPressed, buttonHelpPressed, buttonBadgesPressed, buttonQuitPressed };

	// require all up textures present to draw buttons
	if (btnUp[0].id == 0 || btnUp[1].id == 0 || btnUp[2].id == 0 || btnUp[3].id == 0) {
		// missing textures -> do not draw buttons (user requested no fallbacks)
		return;
	}

	float posX[4], posY[4], w[4], h[4], totalH, startY;
	ComputeButtonLayout(screenW, screenH, btnUp, posX, posY, w, h, totalH, startY, titleBottom);

	// draw each button with uniform scale computed in layout
	for (int i = 0; i < 4; ++i) {
		Texture2D tex = btnUp[i];
		if ((int)selectedOption == i && btnDown[i].id != 0) tex = btnDown[i];

		// determine scale from desired width
		float s = w[i] / (float)tex.width;

		Vector2 pos = { posX[i], posY[i] };
		DrawTextureEx(tex, pos, 0.0f, s, WHITE);

		// draw selection arrow if needed
		if ((int)selectedOption == i && buttonArrow.id != 0) {
			float arrowScale = 1.0f;
			if (buttonArrow.height > 0) arrowScale = (h[i] * 0.6f) / (float)buttonArrow.height;
			if (arrowScale > 1.0f) arrowScale = 1.0f;
			Vector2 arrowPos = { posX[i] - (float)buttonArrow.width * arrowScale - 12.0f,
								 posY[i] + (h[i] / 2.0f) - ((float)buttonArrow.height * arrowScale / 2.0f) };
			DrawTextureEx(buttonArrow, arrowPos, 0.0f, arrowScale, WHITE);
		}
	}
}