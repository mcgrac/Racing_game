#pragma once
#include"EntityManager.h"
#include <vector>
#include <algorithm> // Para std::sort

class Checkpoint;
class Characters;

struct RacePositionData {
    Characters* car;
    float totalProgress; // El valor único para ordenar: Vueltas + Tramo + Distancia
    int currentRacePosition; // 1, 2, 3...

    // Sobrecarga del operador < para ordenar fácilmente con std::sort
    // La clasificación será de MAYOR a menor totalProgress.
    bool operator<(const RacePositionData& other) const {
        return totalProgress > other.totalProgress;
    }
};

class PositionTracker {
public:
    PositionTracker(const std::vector<Checkpoint*>& _allSegments);
    ~PositionTracker();

    // Este método es llamado desde ModuleGame::Update()
    void UpdatePositions(const std::vector<Entity*>& allRaceEntities);

private:
    std::vector<Checkpoint*> allSegments;

    // Calcula la métrica única para clasificar
    float CalculateTotalProgress(Characters* car);
};
