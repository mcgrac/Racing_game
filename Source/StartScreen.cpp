#pragma once

#include "raylib.h"
#include <vector>
#include <string>
#include "Scene.h"
#include "StartScreen.h"

StartScreen::~StartScreen()
{
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

void StartScreen::Update() {
	// Navegación por teclado / gamepad
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

	// Confirmar selección
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
		switch (selectedOption) {
		case START:
			hasPressStart = true;
			break;
		case HELP:
			hasPressHelp = true;
			break;
		case BADGE:
			hasPressBadge = true;
			break;
		case QUIT:
			hasPressQuit = true;
			break;
		default:
			break;
		}
	}

	// Soporta click del ratón sobre las opciones
	Vector2 mouse = GetMousePosition();
	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();
	const int optionCount = MAIN_MENU_OPTION_COUNT;
	const int optionHeight = 48;
	const int spacing = 16;
	int totalHeight = optionCount * optionHeight + (optionCount - 1) * spacing;
	int startY = screenH / 2 - totalHeight / 2;

	for (int i = 0; i < optionCount; ++i) {
		int x = screenW / 2 - 160;
		int y = startY + i * (optionHeight + spacing);
		Rectangle rect = { (float)x, (float)y, 320.0f, (float)optionHeight };
		if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			selectedOption = static_cast<MainMenuOption>(i);
			// marcar como pulsado inmediatamente
			switch (selectedOption) {
			case START: hasPressStart = true; break;
			case HELP: hasPressHelp = true; break;
			case BADGE: hasPressBadge = true; break;
			case QUIT: hasPressQuit = true; break;
			default: break;
			}
		}
	}
}

void StartScreen::Draw() {
	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	// Dibujar background si está cargado, escalándolo a pantalla
	if (backgroundMainMenu.id != 0) {
		Rectangle src = { 0.0f, 0.0f, (float)backgroundMainMenu.width, (float)backgroundMainMenu.height };
		Rectangle dst = { 0.0f, 0.0f, (float)screenW, (float)screenH };
		Vector2 origin = { 0.0f, 0.0f };
		DrawTexturePro(backgroundMainMenu, src, dst, origin, 0.0f, WHITE);
	}
	else {
		ClearBackground(RAYWHITE);
	}

	// Si hay título como textura, dibujarlo centrado en la parte superior (ajusta Y si quieres)
	if (mainMenuTitle.id != 0) {
		int titleW = mainMenuTitle.width;
		int titleH = mainMenuTitle.height;

		// Escalado máximo para que no sea mayor que el 80% del ancho de la pantalla
		float maxWidth = screenW * 0.8f;
		float scale = 1.0f;
		if (titleW > 0 && titleW > maxWidth) scale = maxWidth / (float)titleW;

		float drawW = titleW * scale;
		float drawH = titleH * scale;

		int drawX = screenW / 2 - (int)(drawW / 2);
		int drawY = screenH / 8; // posición por encima del centro

		Vector2 titlePos;
		titlePos.x = (float)drawX;
		titlePos.y = (float)drawY;
		DrawTextureEx(mainMenuTitle, titlePos, 0.0f, scale, WHITE);
	}

	// Opciones del menú (texto en español para la UI)
	std::vector<std::string> options = { "Play", "Help", "Badges", "Quit" };

	const int optionWidth = 320;
	const int optionHeight = 48;
	const int spacing = 16;
	int totalHeight = (int)options.size() * optionHeight + ((int)options.size() - 1) * spacing;
	int startY = screenH / 2 - totalHeight / 2;

	for (int i = 0; i < (int)options.size(); ++i) {
		int x = screenW / 2 - optionWidth / 2;
		int y = startY + i * (optionHeight + spacing);
		Rectangle rect = { (float)x, (float)y, (float)optionWidth, (float)optionHeight };

		bool selected = (i == static_cast<int>(selectedOption));
		Color boxColor = selected ? SKYBLUE : LIGHTGRAY;
		Color borderColor = selected ? BLUE : GRAY;
		Color textColor = selected ? BLACK : DARKGRAY;

		// Fondo de la opción
		DrawRectangleRec(rect, boxColor);
		DrawRectangleLinesEx(rect, 2.0f, borderColor);

		// Texto centrado
		const std::string& txt = options[i];
		int fontSize = 24;
		int textWidth = MeasureText(txt.c_str(), fontSize);
		DrawText(txt.c_str(), x + optionWidth / 2 - textWidth / 2, y + (optionHeight / 2 - fontSize / 2), fontSize, textColor);
	}

}