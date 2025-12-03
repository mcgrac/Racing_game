#pragma once
//#include "Player.h"

class Player;

class Camera {

public:

	Camera(Player* player);
	~Camera();

	void Start();
	void Update();
	void CleanUp();
	
	void FollowPlayer();

protected:


private:
};
