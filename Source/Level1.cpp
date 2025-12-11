#include "Level1.h"


Level1::Level1(ModulePhysics* _physics, Module* _listener, EntityManager* _entityManager) : Map(MapType::LEVEL_1),
	physics(_physics),
	listener(_listener),
	entityManager(_entityManager)
{
}

Level1::~Level1()
{
}

//void Level1::Start()
//{
//	floor = LoadTexture("Assets/Textures/Map/Background.png");
//	overMap = LoadTexture("Assets/Textures/Map/TopElementsMap.png");
//
//	//positions parrilla
//	
//	//entityManager->CreateEntity(EntityType::PLAYER, Vector2D(200,200));
//}

//void Level1::Update()
//{
//
//}

bool Level1::Load()
{
	std::cout << "LOADING LEVEL 1" << std::endl;

	floor = LoadTexture("Assets/Textures/Map/Background.png");
	overMap = LoadTexture("Assets/Textures/Map/TopElementsMap.png");

	InitializeStartingGrid();

	LoadColliders();

	return true;
}

void Level1::RenderBackground()
{
	//draw Basic map
	DrawTexture(floor, 0, 0, WHITE);
}
void Level1::RenderTop()
{
	//draw top elements
	DrawTexture(overMap, 0, 0, WHITE);
}

void Level1::CleanUp()
{
    UnloadTexture(floor);
    UnloadTexture(overMap);

    startingPositions.clear();
}

void Level1::InitializeStartingGrid()
{
    // Limpiar posiciones previas por si acaso
    startingPositions.clear();
    //startingRotations.clear();

    // Definir las 4 posiciones de la parrilla de salida
    // Ajusta estos valores según tu circuito
    // Formato: 2 filas de 2 coches cada una

    // Fila 1 (pole position)
    startingPositions.push_back(Vector2D(500.0f, 300.0f));  // Posición 1 (izquierda)
    startingPositions.push_back(Vector2D(700.0f, 300.0f));  // Posición 2 (derecha)

    // Fila 2
    startingPositions.push_back(Vector2D(500.0f, 500.0f));  // Posición 3 (izquierda)
    startingPositions.push_back(Vector2D(700.0f, 500.0f));  // Posición 4 (derecha)

    // Rotaciones iniciales (todas mirando hacia arriba = 0 grados)
    // Si tu pista va en otra dirección, ajusta estos valores
    //for (int i = 0; i < 4; i++) {
    //    startingRotations.push_back(0.0f);  // 0° = mirando hacia arriba
    //}

    LOG("Starting grid initialized with %d positions", startingPositions.size());
}

void Level1::LoadColliders()
{
    //Ap->physics->CreateChain(.....) (crear colliders del circuito)
}
