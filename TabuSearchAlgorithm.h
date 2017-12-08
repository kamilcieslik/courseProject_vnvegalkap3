//
// Created by mrfarinq on 06.12.17.
//

#ifndef COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H
#define COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H


#include <vector>
#include <list>
#include <string>

class TabuSearchAlgorithm {
    int length;
    int amountOfCities;
    int **matrixOfCities;
    std::vector<int> optimalWay;
    std::list<std::pair<std::pair<int,int>, int>> tabuList;

public:
    TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities);

    void DoCalculations(int tabu_list_size = 10, int neighbours_count = 400, int maximumIterationsWithoutBetterSolution = 100,
                        int maximumAmountOfRestarts = 20, int cadence = 0);

    std::pair<std::vector<int>, int> GetResults();

private:
    std::pair<int, int> swapTwoRandomCities(std::vector<int> &path);

    std::vector<int> GreedyAlgorithm();

    std::vector<int> generateRandomSolution();

    bool checkTabu(std::pair<int, int> &swaped_cities, std::vector<int> &path, int neighbour_cost);

    int countCost(std::vector<int> path);

    bool CriticalEvent(int &iterationsWithoutBetterSolution, int &maximumIterationsWithoutBetterSolution);

    void Restart(std::vector<int> &currentSolution, int &currentCost);
};


#endif //COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H