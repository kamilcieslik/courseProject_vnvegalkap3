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

    void DoCalculations(int tabuListSize = 13,
                        int maximumIterationsWithoutBetterSolution = 1000,
                        int maximumRestarts = 200, int tenure = 10, int maximumRestartsWithoutBetterSolution = 4,
                        std::string neighborhoodType = "insert", bool showIntermediateSolutionsInRuntime = false);

    std::pair<std::vector<int>, int> GetResults();

    const std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> &getIntermediateSolutions() const;

private:
    std::vector<int> GreedyAlgorithm();

    std::vector<int> generateRandomSolution();

    bool
    citiesDoNotExistOnTabuList(std::pair<int, int> &neighborhoodCities, std::vector<int> &tempSolution,
                               long long int neighborhoodCost);

    long long int countCost(std::vector<int> currentSolution);

    bool CriticalEvent(int &iterationsWithoutBetterSolution, int &maximumIterationsWithoutBetterSolution);

    void Restart(std::vector<int> &currentSolution, long long int &currentCost);

    std::list<std::pair<int, int>> findRandomCitiesToNeighbourhood(std::vector<int> &currentSolution,
                                                                   int neighbourhoodSize);

    std::vector<int>
    GenerateBestNewCurrentSolutionFromNeighbourhood(std::vector<int> currentSolution,
                                                    std::list<std::pair<int, int>> neighbours,
                                                    std::pair<int, int> neighborhoodCities,
                                                    std::string neighborhoodType);

    void decrementTenures(std::list<std::pair<std::pair<int, int>, int>> &tabuList);

    void neighbourBySwap(std::vector<int> &tempSolution, long city_1, long city_2);

    void neighbourByInsert(std::vector<int> &tempSolution, long city_1, long city_2);

    void neighbourByInvert(std::vector<int> &tempSolution, long city_1, long city_2);
};


#endif //COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H