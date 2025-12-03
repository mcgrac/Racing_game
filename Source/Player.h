#pragma once
#include "Globals.h"
#include "p2Point.h"
#include "Application.h"

class Player
{
public:
	Player(Application* app, bool start_enabled = true);
	virtual ~Player();

	bool Start();
	void Update();
	bool CleanUp();

};