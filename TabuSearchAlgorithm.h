//
// Created by mrfarinq on 06.12.17.
//

#ifndef COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H
#define COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H


#include <vector>
#include <list>
#include <string>
#include "IntermediateSolutionOfTheTabuSearchAlgorithm.h"

class TabuSearchAlgorithm {
    long long int length;
    int amountOfCities;
    int **matrixOfCities;
    std::vector<int> optimalWay;
    std::list<std::pair<std::pair<int, int>, int>> tabuList;
    std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> intermediateSolutions;

public:
    TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities);

    void DoCalculations(int tabuListSize = 13, int amountOfNeighbours = 200,
                        int maximumIterationsWithoutBetterSolution = 1000,
                        int maximumRestarts = 200, int cadence = 10, int maximumRestartsWithoutBetterSolution = 4, bool isIntensification = false);

    std::pair<std::vector<int>, int> GetResults();

    const std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> &getIntermediateSolutions() const;

private:
    std::vector<int> GreedyAlgorithm();

    std::vector<int> generateRandomSolution();

    bool checkTabu(std::pair<int, int> &swaped_cities, std::vector<int> &path, long long int neighbour_cost);

    long long int countCost(std::vector<int> path);

    bool CriticalEvent(int &iterationsWithoutBetterSolution, int &maximumIterationsWithoutBetterSolution);

    void Restart(std::vector<int> &currentSolution, long long int &currentCost);

    std::list<std::pair<int, int>> findRandomNeighbourhood(std::vector<int> &solution, int neighbourhoodSize);

    std::vector<int>
    aspirationPlus(std::vector<int> solution, std::list<std::pair<int, int>> neighbour, std::pair<int, int> pair);

    void decrementCadences(std::list<std::pair<std::pair<int,int>, int>> &tabuList);
};


#endif //COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H