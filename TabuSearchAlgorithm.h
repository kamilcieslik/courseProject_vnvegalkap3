//
// Created by mrfarinq on 06.12.17.
//

#ifndef COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H
#define COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H


#include <vector>

class TabuSearchAlgorithm {
    int length;
    int amountOfCities;
    int **matrixOfCities;
    std::vector<int> optimalWay;

public:
    TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities);

    ~TabuSearchAlgorithm();

    void DoCalculations();

    std::pair<std::vector<int>, int> GetResults();
};


#endif //COURSEPROJECT_PEA2_TABUSEARCHALGORITHM_H
