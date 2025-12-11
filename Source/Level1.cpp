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

    //textures load
	floor = LoadTexture("Assets/Textures/Map/Background.png");
	overMap = LoadTexture("Assets/Textures/Map/TopElementsMap.png");
    
    //sounds load
    inGameMusic = LoadSound("Assets/Sound/Music/InGameMusicEncore.wav");
    inGameMusicBeggining = LoadSound("Assets/Sound/Music/InGameMusicBeggining.wav");

	InitializeStartingGrid();
	LoadColliders("Assets/Coordinates.txt");
    LoadBoosts();

    PlaySound(inGameMusicBeggining); //play the intro of the level
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

    UnloadSound(inGameMusic);
    UnloadSound(inGameMusicBeggining);

    startingPositions.clear();
}

void Level1::UpdateMusic()
{
    if (!IsSoundPlaying(inGameMusicBeggining)) {
        if(!IsSoundPlaying(inGameMusic))
        PlaySound(inGameMusic);
    }
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
    startingPositions.emplace_back(Vector2D(1220.0f, 1980.0f));  // Posición 1 (izquierda)
    startingPositions.emplace_back(Vector2D(1350.0f, 1980.0f));  // Posición 2 (derecha)

    // Fila 2
    startingPositions.emplace_back(Vector2D(1220.0f, 2110.0f));  // Posición 3 (izquierda)
    startingPositions.emplace_back(Vector2D(1350.0f, 2110.0f));  // Posición 4 (derecha)

    // Rotaciones iniciales (todas mirando hacia arriba = 0 grados)
    // Si tu pista va en otra dirección, ajusta estos valores
    //for (int i = 0; i < 4; i++) {
    //    startingRotations.push_back(0.0f);  // 0° = mirando hacia arriba
    //}

    LOG("Starting grid initialized with %d positions", startingPositions.size());
}

void Level1::LoadColliders(const char* filePath)
{
    //Ap->physics->CreateChain(.....) (crear colliders del circuito)
    LoadAllChains(filePath);
}

void Level1::LoadBoosts()
{
    //create boosts
    Boost* b = new Boost(listener, Vector2D(1300.0f, 1700.0f), EntityType::TURBO_ON_ROAD, PhysicCategory::SENSORS, PhysicCategory::CARS);
    boostsList.emplace_back(b);
    entityManager->AddEntity(b);

    
}

void Level1 :: LoadAllChains(const char* filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return;
    }
    std::vector<int> pts;

    int x, y;
    std::string line;
    while (std::getline(file, line))
    {
        // Eliminar espacios/tabs al inicio y final
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line == "END")
        {
            // Crear el collider con pts
            physics->CreatePolygon(0, 0, pts.data(), pts.size(), b2_staticBody, PhysicCategory::WALLS, PhysicCategory::CARS, 0);
            pts.clear();
            continue;
        }

        std::stringstream ss(line);
        if (ss >> x >> y)
        {
            pts.push_back(x);
            pts.push_back(y);
        }
        else {
            std::cerr << "No se pudieron leer coordenadas en la línea: " << line << std::endl;
        }
    }
}
