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
    std::list<std::pair<int, int> > tabu_list;
    std::vector<int> optimalWay;

public:
    TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities);

    void DoCalculations(int tabu_list_size = 10, int neighbours_count = 400, int steps_without_change = 100,
                        int steps_with_random_neighbourhood = 20);

    std::pair<std::vector<int>, int> GetResults();

private:
    std::pair<int, int> swapTwoRandomCities(std::vector<int> &path);

    std::vector<int> generateRandomSolution();

    bool checkTabu(std::pair<int, int> &swaped_cities, std::vector<int> &path, int neighbour_cost);

    int countCost(std::vector<int> path);
};


#endif //COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H
