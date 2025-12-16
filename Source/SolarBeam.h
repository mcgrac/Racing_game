#pragma once
#include "Entity.h"


class SolarBeam : public Entity {
public:

	SolarBeam(Module* _listener, const Vector2D& pos, EntityType _type, uint16 category, uint16 maskBits, float _rotation, int16 groupIndex = 0);
	~SolarBeam();

	bool Update(float dt) override;
	bool Render() override;
	bool CleanUp() override;

	inline void SetRotation(float f) { rotationBeam = f; }

	void SetPositionAndRotation(const Vector2D& carPos, float carRotation);

private:
	void LoadAnimations();
	void InitPhysics(uint16 category, uint16 maskBits, int16 groupIndex);

	Animation solarBeam;
	float rotationBeam;
};

