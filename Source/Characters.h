#pragma once
#include"Entity.h"


class Characters :public Entity {

public:
	Characters(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);

	inline void SetIsPlayer(bool b) { isPlayer = b; }

	//wayPoints IA
	void SetWaypoints(const std::vector<Vector2D>& points);
	const std::vector<Vector2D>& GetWaypoints() const { return waypoints; }
	int GetCurrentWaypointIndex() const { return currentWaypointIndex; }
	Vector2D GetCurrentWaypoint() const;
	Vector2D GetNextWaypoint() const;
	void AdvanceToNextWaypoint();
	void ResetWaypoints();
	//read waypoints
	void WaypointLoader(const char* path);

	//getters/setters
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
	inline void SetAcceleration(float acc) {
		accelerationForce = acc;
	}
	inline void SetMaxSpeed(float MaxS) {
		maxForwardSpeed = MaxS;
	}
	inline void SetIsBoosted(bool b) {
		isBoosted = b;
	}
	inline void SetOffRoad(int road) {
		stats.offRoad = road;
	}
	inline void SetTurboPower(float f) {
		turboPower = f;
	}

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

	enum class State {
		IDLE,
		ATTACK,
		STUNNED,
	};
	State currentState;
	State previousState;
	float stateTimer;

	//variables
	float speed;
	bool textureLoaded;

	//control
	bool isPlayer;

	//boost
	bool isBoosted;
	float turboPower;
	float boostTimer;

	// Car dimensions
	float width;
	float height;

	//Car physics variables
	float maxForwardSpeed;      // Velocidad máxima hacia adelante (m/s)
	float maxBackwardSpeed;     // Velocidad máxima marcha atrás (m/s)
	float accelerationForce;    // Fuerza de aceleración
	float brakeForce;           // Fuerza de frenado
	float turnTorque;           // Torque de giro (rota el physBody)
	float dragCoefficient;      // Resistencia del aire
	float lateralDrag;          // Fricción lateral (anti-drift)
	float minSpeedToTurn;       // Velocidad mínima para poder girar
	float rotation;

	//Animations
	Animation idleAnimation;
	Animation stunnedAnimation;
	Animation attackAnimation;

	//IA
	std::vector<Vector2D> waypoints;
	int currentWaypointIndex;
	float waypointReachRadius;  // Radio para considerar alcanzado un waypoint
	bool loopWaypoints;  // true = circuito cerrado, false = ir y volver

	void UpdateState(float dt);
	virtual void UpdateAnims(float dt) = 0;
	virtual void Boost(float dt) = 0;
};