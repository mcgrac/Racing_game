#include "Characters.h"
#include "Checkpoint.h"

Characters::Characters(Module* _listener, const Vector2D& startPos, EntityType _type, uint16 category, uint16 maskBits, int16 groupIndex) :
	Entity(_listener, startPos, _type),
	idleAnimation(0.15f), //0.15 seconds per frame
	attackAnimation(0.2f), //0.2 seconds per frame
	stunnedAnimation(0.2f), //0.2 seconds per frame
	preparingAttack (0.2f),
	speed(200.0f),
	textureLoaded(false),
	maxForwardSpeed(10.0f),        // 72 km/h
	maxBackwardSpeed(25.0f),        // 28.8 km/h
	accelerationForce(63.0f),      // Fuerza de aceleración
	brakeForce(40.0f),             // Fuerza de frenado
	turnTorque(15.0f),             // Torque de giro
	dragCoefficient(1.1f),         // Resistencia del aire
	lateralDrag(1.5f),             // Fricción lateral
	minSpeedToTurn(0.5f),           // Velocidad mínima para girar
	rotation(0.0f),
	currentWaypointIndex(0),
	waypointReachRadius(100.0f),  // 50 píxeles de radio
	loopWaypoints(true),
	isBoosted(false),
	boostTimer(0.0f),
	turboPower(0.0f),
	checkpointArrived(0),
	isOffRoad(false)
{
	currentState = State::IDLE;
	previousState = State::IDLE;
	stateTimer = 0.0f;
}

Characters::~Characters()
{
}

void Characters::UpdateState(float dt)
{
	//if there is a tempoal state like stunned
	if (stateTimer > 0.0f) { 
		stateTimer -= dt; 
		
		if (stateTimer <= 0.0f) {

			currentState = State::IDLE;
			std::cout << "Player returned to idle" << std::endl;
			stateTimer = 0.0f;

			//reset forward speed
			std::cout << "Previous max speed: " << maxForwardSpeed << std::endl;
			maxForwardSpeed *= 2;
			std::cout << "New max speed: "<<maxForwardSpeed << std::endl;
		}
	} 
}

void Characters::WaypointLoader(const char* path)
{
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo: " << path << std::endl;
		return;
	}

	waypoints.clear();

	std::vector<int> pts;

	int x, y;
	std::string line;
	int lineNumber = 0;
	while (std::getline(file, line))
	{
		lineNumber++;

		// Eliminar espacios/tabs al inicio y final
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);
		
		if (line.empty())
		{
			continue;
		}

		if (line[0] == '#') {
			continue;
		}

		// Leer coordenadas x y
		std::stringstream ss(line);
		if (ss >> x >> y)
		{
			waypoints.push_back(Vector2D((float)x, (float)y));
		}
		else {
			std::cerr << "No se pudieron leer coordenadas en la línea " << lineNumber << ": " << line << std::endl;
		}
	}

	file.close();

	// Reset al primer waypoint
	currentWaypointIndex = 0;

	std::cout << "=== WAYPOINTS LOADED ===" << std::endl;
	for (size_t i = 0; i < std::min(waypoints.size(), size_t(5)); i++) {
		std::cout << "  [" << i << "] (" << waypoints[i].getX() << ", " << waypoints[i].getY() << ")" << std::endl;
	}
	if (waypoints.size() > 5) {
		std::cout << "  ... and " << (waypoints.size() - 5) << " more waypoints" << std::endl;
	}
	std::cout << "========================" << std::endl;
}

float Characters::CalculateDistanceFromCheckpoint(Checkpoint* ch)
{
	Vector2D posCar = position;
	return posCar.distanceEuclidean(ch->GetCenter());
}

#pragma region HELPERS
b2Vec2 Characters::GetForwardVector() const {
	float angle = physBody->body->GetAngle();
	return b2Vec2(sinf(angle), -cosf(angle));
}
b2Vec2 Characters::GetRightVector() const {
	b2Vec2 forward = GetForwardVector();
	return b2Vec2(-forward.y, forward.x);
}
#pragma endregion



#pragma region WAYPOINT SYSTEM
void Characters::SetWaypoints(const std::vector<Vector2D>& points)
{
	waypoints = points;
	currentWaypointIndex = 0;
	LOG("Waypoints set: %d points", waypoints.size());
}

Vector2D Characters::GetCurrentWaypoint() const
{
	if (waypoints.empty()) {
		return Vector2D(0, 0);
	}
	return waypoints[currentWaypointIndex];
}

Vector2D Characters::GetNextWaypoint() const
{
	if (waypoints.empty()) {
		return Vector2D(0, 0);
	}

	int nextIndex = (currentWaypointIndex + 1) % waypoints.size();
	return waypoints[nextIndex];
}

void Characters::AdvanceToNextWaypoint()
{
	if (waypoints.empty()) return;

	currentWaypointIndex++;

	if (loopWaypoints) {
		// Modo circuito cerrado
		currentWaypointIndex %= waypoints.size();
	}
	else {
		// Modo ida y vuelta
		if (currentWaypointIndex >= waypoints.size()) {
			currentWaypointIndex = 0;
		}
	}

	LOG("Advanced to waypoint %d", currentWaypointIndex);
}

void Characters::ResetWaypoints()
{
	currentWaypointIndex = 0;
}
#pragma endregion
