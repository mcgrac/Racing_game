#include "PositionTracker.h"
#include "Characters.h"

PositionTracker::PositionTracker(const std::vector<Checkpoint*>& _allSegments): allSegments(_allSegments)
{

}

PositionTracker::~PositionTracker()
{

}
void PositionTracker::UpdatePositions(const std::vector<Entity*>& allRaceEntities)
{
    std::vector<RacePositionData> raceData;
    //std::cout << "\n--- INICIO CALCULO DE POSICIONES ---" << std::endl;

    // 1. Recopilar datos y calcular el Progreso Total para cada coche
    for (Entity* entity : allRaceEntities) {
        Characters* car = dynamic_cast<Characters*>(entity);
        if (car) {
            RacePositionData data;
            data.car = car;
            data.totalProgress = CalculateTotalProgress(car);
            raceData.push_back(data);
        }
    }

    // 2. Ordenar: El coche con mayor totalProgress es el que va primero
    std::sort(raceData.begin(), raceData.end());

    //std::cout << "\n--- RESULTADO DE LA ORDENACION ---" << std::endl;

    // 3. Asignar la Posición en la Carrera
    for (size_t i = 0; i < raceData.size(); ++i) {
        int position = (int)i + 1;
        raceData[i].currentRacePosition = position;

        // Actualizar la variable positionInRace en el Character
        raceData[i].car->SetPositionInRace(position);

        // DEBUG: Mostrar la posición final asignada
        //std::cout << "| POSICION #" << position
        //    << " | Car IsPlayer: " << raceData[i].car->GetIsPlayer() // Asumiendo que Character tiene un ID
        //    << " | Progreso Total: " << std::fixed << raceData[i].totalProgress
        //    << " |" << std::endl;
    }
}

float PositionTracker::CalculateTotalProgress(Characters* car)
{
    int segmentID = car->GetCheckId(); //gets the last checkpoint of the car

    // Esto asume que los IDs están en orden 0, 1, 2...
    if (segmentID >= allSegments.size()) {
        return 0.0f;
    }

    Checkpoint* currentSegment = allSegments[segmentID];
    float distanceInSegment = car->CalculateDistanceFromCheckpoint(currentSegment);

    float totalProgress = ((float)car->GetLaps() * 10000.0f) //laps are valued 10000
        +
        (float)segmentID * 500.0f  //segments are valued 200
        +
        distanceInSegment; //finally add the distance from the segment

    return totalProgress;
}