#pragma once
#include"Entity.h"

class Checkpoint;

class Characters :public Entity {

public:
	Characters(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex = 0);
	~Characters();

	enum class State {
		IDLE,
		ATTACK,
		STUNNED,
		PREPARING_ATTACK
	};
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
	inline float GetMaxSpeed() {
		return maxForwardSpeed;
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
	inline void SetTurboPower(float f) {
		turboPower = f;
	}
	inline void SetCheckpointArrived(int i) { checkpointArrived = i; }
	inline void SetPositionInRace(int i) { positionInRace = i; }
	inline void SetStunnedState() { currentState = State::STUNNED; }
	inline void SetStateTimer(float f) { stateTimer = f; }
	inline void SetIsOffRoad(bool b) { isOffRoad = b; }

	inline void AddOneLap() { laps++; }

	inline int GetCheckId() { return checkpointArrived; }
	inline int GetLaps() { return laps; }
	inline int GetPositionInRace() { return positionInRace; }
	inline bool GetIsPlayer() { return isPlayer; }
	inline int GetCurrentState() const { return (int)currentState; }
	inline bool GetIsOffRoad() { return isOffRoad; }
	inline Sound GetWallBumpSound() { return wallBump; }

	float CalculateDistanceFromCheckpoint(Checkpoint* ch);

protected:

#pragma region GETTERS/SETTERS

	inline float GetSpeed() const { return physBody->body->GetLinearVelocity().Length(); };
#pragma endregion

	struct Statistics {
		int acceleration;
		int maxSpeed;
		int turbo;
		int offRoad;
	};
	Statistics stats;
	//afegir tilset
	
	Texture texture;
	//Icones portraits
	Texture2D neutral_portrait;
	Texture2D happy_portrait;
	Texture2D stunned_portrait;
	Texture2D sad_portrait;

	void LoadPortraits();


	State currentState;
	State previousState;
	float stateTimer;

	//variables
	float speed;
	bool textureLoaded;
	bool isOffRoad;

	//position in the race
	int positionInRace;
	int checkpointArrived;
	int laps;

	//score
	float timeRace;

	//boost
	bool isBoosted;
	float turboPower;
	float boostTimer;

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
	Animation preparingAttack;

	//IA
	std::vector<Vector2D> waypoints;
	int currentWaypointIndex;
	float waypointReachRadius;  // Radio para considerar alcanzado un waypoint
	bool loopWaypoints;  // true = circuito cerrado, false = ir y volver

	void UpdateState(float dt);
	virtual void UpdateAnims(float dt) = 0;
	virtual void Boost(float dt) = 0;

	//sounds
	Sound accelerate;
	Sound attackSound;
	Sound wallBump;

	//Helpers
	b2Vec2 GetForwardVector() const;
	b2Vec2 GetRightVector() const;
};