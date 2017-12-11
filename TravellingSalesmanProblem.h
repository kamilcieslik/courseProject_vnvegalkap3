//
// Created by mrfarinq on 16.06.17.
//

#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <queue>
#include <algorithm>
#include <map>
#include "IntermediateSolutionOfTheTabuSearchAlgorithm.h"

class TravellingSalesmanProblem {

private:
    int amountOfCities;
    int **matrixOfCities;
    std::vector<int> optimalWay;
    long long int optimalLength;
    std::string fileName;
    std::string graphType;
    bool randomGeneratorData;
    std::string whichTypeOfAlgorithm;
    std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> intermediateSolutions;

public:
    TravellingSalesmanProblem();

    ~TravellingSalesmanProblem();

    void DeleteTravellingSalesman();

    void LoadArrayOfMatrixOfCities(int **_cities, int _amountOfCities,
                                   std::string _fileName, std::string _graphType);

    void ReadCitiesFromNormalFile(std::string path);

    void GenerateRandomCities(int amountOfCities = 0, int maxDistanceBetweenCity = 99);

    void PrintCitiesForTheTravellingSalesman(bool printInputMatrix);

    void PerformBruteForceAlgorithm();

    void PerformBranchAndBoundAlgorithm();

    void
    PerformTabuSearchAlgorithm(std::string neighborhoodType = "auto", bool showIntermediateSolutionsInRuntime = false);

    void PrintSolution();

    long long int GetTourLength();
};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
