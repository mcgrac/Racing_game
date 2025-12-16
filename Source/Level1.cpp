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

    //clear choosen pokemon list
    GetPokemonsTakenList().clear();

	InitializeStartingGrid();
	LoadColliders("Assets/Coordinates.txt");
    LoadBoosts("Assets/Boosts.txt");
    LoadCheckpoints("Assets/Sectors.txt");
    LoadOffRoadSensors("Assets/Off_Road_zones.txt");
    LoadRocks("Assets/RocksPositions.txt");

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

    // Definir las 4 posiciones de la parrilla de salida
    // Formato: 2 filas de 2 coches cada una

    // Fila 1 (pole position)
    startingPositions.emplace_back(Vector2D(1220.0f, 1980.0f));  // Posición 1 (izquierda)
    startingPositions.emplace_back(Vector2D(1350.0f, 1980.0f));  // Posición 2 (derecha)

    // Fila 2
    startingPositions.emplace_back(Vector2D(1220.0f, 2110.0f));  // Posición 3 (izquierda)
    startingPositions.emplace_back(Vector2D(1350.0f, 2110.0f));  // Posición 4 (derecha)

    LOG("Starting grid initialized with %d positions", startingPositions.size());
}

void Level1::LoadColliders(const char* filePath)
{
    //Ap->physics->CreateChain(.....) (crear colliders del circuito)
    LoadAllChains(filePath);
}

void Level1::LoadBoosts(const char* filePath)
{

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return;
    }

    int x, y;
    float rotation;
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {

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
        if (ss >> x >> y >> rotation)
        {
            Boost* b = new Boost(listener, Vector2D(x, y), rotation, EntityType::TURBO_ON_ROAD, PhysicCategory::SENSORS, PhysicCategory::CARS);
            boostsList.emplace_back(b);
            entityManager->AddEntity(b);
        }
        else {
            std::cerr << "No se pudieron leer coordenadas en la línea " << lineNumber << ": " << line << std::endl;
        }

    }
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

void Level1::LoadCheckpoints(const char* filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return;
    }
    std::vector<int> pts;
    int x, y;

    std::string line;
    int currentId = 0; // Para asignar un ID único a cada checkpoint

    while (std::getline(file, line)) {

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

        if (line == "END")
        {
            Checkpoint* ch = new Checkpoint(listener, Vector2D{ 0,0 }, EntityType::CHECKPOINT, currentId, PhysicCategory::CHECKPOINTS, PhysicCategory::AI | PhysicCategory::CARS, 0, pts.data(), pts.size());
            checkpointsList.emplace_back(ch);

            pts.clear();
            currentId++;
            continue;
        }

        std::stringstream ss(line);
        if (ss >> x >> y) {

            pts.emplace_back(x);
            pts.emplace_back(y);
        }
        else {
            // Error al leer la línea (quizás está vacía o mal formateada)
            std::cerr << "Advertencia: Línea mal formateada o vacía en el archivo de checkpoints: " << line << std::endl;
        }
    }

    // El archivo se cierra automáticamente al salir del scope (RAII)
    std::cout << "\nCarga de Checkpoints Completa." << std::endl;
    std::cout << "-> Checkpoints creados: " << currentId << std::endl;
    std::cout << "-> Total de tramos en checkpointsList: " << checkpointsList.size() << std::endl;

    // El archivo se cierra automáticamente al salir del scope (RAII)
    std::cout << "Cargados " << currentId << " checkpoints desde " << filePath << std::endl;
}

void Level1::LoadOffRoadSensors(const char* filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return;
    }
    std::vector<int> pts;
    int x, y;

    std::string line;

    while (std::getline(file, line)) {

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

        if (line == "END")
        {
            Off_road* o;
            o = new Off_road(listener, Vector2D(0.0f, 0.0f), EntityType::OFF_ROAD, PhysicCategory::SENSORS, PhysicCategory::CARS, pts.data(), pts.size(), 0);
            offRoadList.emplace_back(o);
            pts.clear();
            continue;
        }

        std::stringstream ss(line);
        if (ss >> x >> y) {

            pts.emplace_back(x);
            pts.emplace_back(y);
        }
        else {
            // Error al leer la línea (quizás está vacía o mal formateada)
            std::cerr << "Advertencia: Línea mal formateada o vacía en el archivo de checkpoints: " << line << std::endl;
        }
    }

    // El archivo se cierra automáticamente al salir del scope (RAII)
    std::cout << "\nCarga de Off_road Completa." << std::endl;
    std::cout << "-> Total de tramos en Off_road: " << offRoadList.size() << std::endl;

}

void Level1::LoadRocks(const char* filePath)
{

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return;
    }

    int x, y;
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {

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
            Rock* r = new Rock(listener, Vector2D(x, y), EntityType::ROCK, PhysicCategory::DESTRUCTIBLE, PhysicCategory::CARS);
            rocksList.emplace_back(r);
            entityManager->AddEntity(r);
        }
        else {
            std::cerr << "No se pudieron leer coordenadas en la línea " << lineNumber << ": " << line << std::endl;
        }

    }
}

