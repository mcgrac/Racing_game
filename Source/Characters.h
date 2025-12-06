#pragma once
#include"Entity.h"


class Characters :public Entity {
	Characters();
protected:
	struct Statistics {
		int acceleration;
		int maxSpeed;
		int turbo;
		int offRoad;
	};
	Statistics stats;
	//afegir tilset
	//afegir icona

	//void function
	void usePower();
	void iniciate();


public:
	inline int getAcceleration() {
		return stats.acceleration;
	}
	inline int getMaxSpeed() {
		return stats.maxSpeed;
	}
	inline int getTurbo() {
		return stats.turbo;
	}
	inline int getOffRoad() {
		return stats.offRoad;
	}
	inline void setAcceleration(int acc) {
		stats.acceleration = acc;
	}
	inline void setMaxSpeed(int MaxS) {
		stats.maxSpeed = MaxS;
	}
	inline void setTurbo(int turbo) {
		stats.turbo = turbo;
	}
	inline void setOffRoad(int road) {
		stats.offRoad = road;
	}
};
