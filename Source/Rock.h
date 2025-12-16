#pragma once
#include"Interactables.h"
 
class Rock :public Interactables {
public:
	Rock(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~Rock();

	bool Update(float dt) override;
	bool Render() override;
	bool Destroy() override;
	bool CleanUp() override;
private:

	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex) override;
	void LoadAnimations();

	Animation anim;
	Texture2D idle;
};
