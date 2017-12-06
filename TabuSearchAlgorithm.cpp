//
// Created by mrfarinq on 06.12.17.
//

#include "TabuSearchAlgorithm.h"

TabuSearchAlgorithm::TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(matrixOfCities),
                                                                                     amountOfCities(amountOfCities) {
    // TODO Tabu Search Algorithm
}

TabuSearchAlgorithm::~TabuSearchAlgorithm() {
    // TODO Tabu Search Algorithm
}

void TabuSearchAlgorithm::DoCalculations() {
    // TODO Tabu Search Algorithm
}

std::pair<std::vector<int>, int> TabuSearchAlgorithm::GetResults() {
    return std::make_pair(optimalWay, length);
}
