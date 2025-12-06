#include "Globals.h"
#include "Application.h"
#include "Player.h"

Player::Player(Application* app, bool start_enabled)
{
}

Player::~Player()
{}

// Load assets
bool Player::Start()
{
	LOG("Loading player");
	return true;
}

// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
void Player::Update()
{

}



