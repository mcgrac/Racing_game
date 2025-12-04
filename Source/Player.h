#pragma once
#include "Globals.h"
#include "p2Point.h"
#include "Application.h"
#include "Entity.h"

#include"raylib.h"

class Player : public Entity
{
public:
	Player(Application* app, bool start_enabled = true);
	Player();
	Player(float startX, float startY, const char* texturePath);
	Player(const Vector2D& startPos, const char* texturePath);
	Player(const Vector2D& startPos);

	virtual ~Player();

	bool Start() override;
	bool Update(float dt) override;
	void Render(); //testing

	bool CleanUp() override;

	// Getters for dimensions
	float GetWidth() const { return texture.width; }
	float GetHeight() const { return texture.height; }

	// centerd position
	Vector2D GetCenter() const;
	float GetCenterX() const { return position.getX() + GetWidth() / 2.0f; }
	float GetCenterY() const { return position.getY() + GetHeight()/ 2.0f; }

	// texture getters
	const Texture2D& GetTexture() const { return texture; }
	bool IsTextureLoaded() const { return textureLoaded; }

	bool LoadTexture(const char* texturePath);
	void UnloadTexture();

private:

	float speed;
	Texture texture;
	bool textureLoaded;
};