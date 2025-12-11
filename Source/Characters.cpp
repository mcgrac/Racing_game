#include "Characters.h"

Characters::Characters(Module* _listener, const Vector2D& startPos, EntityType _type) :
	Entity(_listener, startPos, _type),
	idleAnimation(0.15f), //0.15 seconds per frame
	attackAnimation(0.2f), //0.2 seconds per frame
	stunnedAnimation(0.2f), //0.2 seconds per frame
	speed(200.0f),
	textureLoaded(false),
	width(0.0f),           // Ancho del rectángulo del coche
	height(0.0f),         // Alto del rectángulo del coche
	maxForwardSpeed(30.0f),        // 72 km/h
	maxBackwardSpeed(25.0f),        // 28.8 km/h
	accelerationForce(65.0f),      // Fuerza de aceleración
	brakeForce(40.0f),             // Fuerza de frenado
	turnTorque(20.0f),             // Torque de giro
	dragCoefficient(1.2f),         // Resistencia del aire
	lateralDrag(1.5f),             // Fricción lateral
	minSpeedToTurn(0.5f),           // Velocidad mínima para girar
	rotation(0.0f)
{
	currentState = State::IDLE;
	previousState = State::IDLE;
	stateTimer = 0.0f;
}

void Characters::UpdateState(float dt)
{
	//if there is a tempoal state like stunned
	if (stateTimer > 0.0f) { 
		
		stateTimer -= dt; 
		
		if (stateTimer <= 0.0f) {

			currentState = State::IDLE;
			std::cout << "Player returned to idle" << std::endl;
		}
	} 
}
