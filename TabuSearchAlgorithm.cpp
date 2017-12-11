//
// Created by mrfarinq on 06.12.17.
//

#include <algorithm>
#include <random>
#include <climits>
#include <iomanip>
#include "TabuSearchAlgorithm.h"
#include "TimeMeasurement.h"

TabuSearchAlgorithm::TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(matrixOfCities),
                                                                                     amountOfCities(amountOfCities) {
}

void TabuSearchAlgorithm::DoCalculations(int tabuListSize,
                                         int maximumIterationsWithoutBetterSolution,
                                         int maximumRestarts, int tenure, int maximumRestartsWithoutBetterSolution,
                                         std::string neighborhoodType,
                                         bool showIntermediateSolutionsInRuntime) {
    TimeMeasurement timeMeasurement;
    timeMeasurement.TimeStart();

    srand(static_cast<unsigned int>(time(nullptr)));

    long long int currentCost;
    std::vector<int> currentSolution(GreedyAlgorithm());
    currentCost = countCost(currentSolution);

    optimalWay = currentSolution;
    length = currentCost;
    intermediateSolutions.push_back(
            IntermediateSolutionOfTheTabuSearchAlgorithm(timeMeasurement.GetTimeInSeconds(), length));

    std::vector<int> neighbour;
    std::pair<int, int> replacedCities;
    std::list<std::vector<int>> goodSolutions;

    int amountOfNeighbours = amountOfCities * 2;
    int amountOfIterations = 0;
    int amountOfRestarts = 0;
    int iterationsWithoutBetterSolution = 0;
    int restartsWithoutBetterSolution = 0;
    bool foundBetterSolutionAfterRestart = false;

    while (amountOfRestarts < maximumRestarts) {
        std::list<std::pair<int, int>> neighbourhood = findRandomCitiesToNeighbourhood(currentSolution,
                                                                                       amountOfNeighbours);

        currentSolution = GenerateBestNewCurrentSolutionFromNeighbourhood(currentSolution, neighbourhood,
                                                                          replacedCities, neighborhoodType);
        currentCost = countCost(currentSolution);

        if (currentCost < length) {
            optimalWay = currentSolution;
            length = currentCost;
            intermediateSolutions.push_back(
                    IntermediateSolutionOfTheTabuSearchAlgorithm(timeMeasurement.GetTimeInSeconds(), length));
            if (showIntermediateSolutionsInRuntime) {
                std::cout << std::fixed << std::setprecision(6)
                          << intermediateSolutions.back().getTimeFromTheBeginningOfTheAlgorithm() << "\t|\t"
                          << intermediateSolutions.back().getActualResult() << std::endl;
            }
            iterationsWithoutBetterSolution = 0;
            foundBetterSolutionAfterRestart = true;
        } else
            iterationsWithoutBetterSolution++;

        if (tabuList.size() > tabuListSize) {
            tabuList.pop_front();
        }

        std::pair<std::pair<int, int>, int> tabuMovement{replacedCities, tenure};
        tabuList.push_back(tabuMovement);
        decrementTenures(tabuList);

        if (restartsWithoutBetterSolution == maximumRestartsWithoutBetterSolution) {
            currentSolution = optimalWay;
            restartsWithoutBetterSolution = 0;
            foundBetterSolutionAfterRestart = true;
        }

        if (CriticalEvent(iterationsWithoutBetterSolution, maximumIterationsWithoutBetterSolution)) {
            if (!foundBetterSolutionAfterRestart)
                restartsWithoutBetterSolution++;
            else
                restartsWithoutBetterSolution = 0;

            foundBetterSolutionAfterRestart = false;
            Restart(currentSolution, currentCost);
            iterationsWithoutBetterSolution = 0;
            amountOfRestarts++;
        }
        amountOfIterations++;
    }
}

std::vector<int> TabuSearchAlgorithm::GreedyAlgorithm() {
    std::vector<int> solution;
    for (int i = 0; i < amountOfCities; i++) {
        solution.push_back(i);
    }

    bool *visitedCities = new bool[amountOfCities];
    for (int i = 0; i < amountOfCities; i++) {
        visitedCities[i] = false;
    }

    length = 0;
    int currentMinLength;

    int nextCity = 0;
    int city = nextCity;
    visitedCities[city] = true;

    solution[0] = nextCity;

    for (auto j = 0; j < amountOfCities - 1; j++) {
        city = nextCity;
        currentMinLength = INT_MAX;
        for (auto i = 0; i < amountOfCities; i++) {
            if (matrixOfCities[city][i] < currentMinLength && !visitedCities[i]) {
                currentMinLength = matrixOfCities[city][i];
                nextCity = i;
            }
        }
        visitedCities[nextCity] = true;
        solution[j] = nextCity;
        length += matrixOfCities[city][nextCity];
    }
    solution[amountOfCities - 1] = 0;
    length += matrixOfCities[solution[amountOfCities - 2]][0];

    delete[] visitedCities;

    return solution;
}

std::vector<int> TabuSearchAlgorithm::generateRandomSolution() {
    std::vector<int> solution;
    for (auto i = 0; i < amountOfCities; i++) {
        solution.push_back(i);
    }
    std::shuffle(solution.begin(), solution.end(), std::mt19937(std::random_device()()));
    return solution;
}

long long int TabuSearchAlgorithm::countCost(std::vector<int> currentSolution) {
    long long int cost = 0;
    for (std::size_t i = 0; i < currentSolution.size() - 1; i++) {
        cost += matrixOfCities[currentSolution[i]][currentSolution[i + 1]];
    }
    cost += matrixOfCities[currentSolution[currentSolution.size() - 1]][currentSolution[0]];

    return cost;
}

bool TabuSearchAlgorithm::citiesDoNotExistOnTabuList(std::pair<int, int> &neighborhoodCities,
                                                     std::vector<int> &tempSolution,
                                                     long long int neighborhoodCost) {
    auto iterator = tabuList.begin();
    while (iterator != tabuList.end()) {
        if (neighborhoodCities.first == iterator->first.second ||
            neighborhoodCities.second == iterator->first.first) {
            long city_1Position =
                    std::find(tempSolution.begin(), tempSolution.end(), iterator->first.first) - tempSolution.begin();
            long city_2Position =
                    std::find(tempSolution.begin(), tempSolution.end(), iterator->first.second) - tempSolution.begin();
            if (neighborhoodCost > length && city_1Position < city_2Position)
                return false;
        }
        iterator++;
    }
    return true;
}

std::pair<std::vector<int>, int> TabuSearchAlgorithm::GetResults() {
    return std::make_pair(optimalWay, length);
}

bool
TabuSearchAlgorithm::CriticalEvent(int &iterationsWithoutBetterSolution, int &maximumIterationsWithoutBetterSolution) {
    return iterationsWithoutBetterSolution > maximumIterationsWithoutBetterSolution;
}

void TabuSearchAlgorithm::Restart(std::vector<int> &currentSolution, long long int &currentCost) {
    currentSolution = generateRandomSolution();
    currentCost = countCost(currentSolution);
    long long int newCost;
    std::vector<int> newRandomSolution;
    for (auto i = 0; i < 10; i++) {
        newRandomSolution = generateRandomSolution();
        newCost = countCost(newRandomSolution);
        if (newCost < currentCost) {
            currentSolution = newRandomSolution;
            currentCost = newCost;
        }
    }
}

std::list<std::pair<int, int>>
TabuSearchAlgorithm::findRandomCitiesToNeighbourhood(std::vector<int> &currentSolution, int neighbourhoodSize) {
    std::list<std::pair<int, int>> randomCitiesToNeighbourhood;

    for (auto i = 0; i < neighbourhoodSize; i++) {
        auto city_1 = rand() % currentSolution.size();

        unsigned long city_2;
        do {
            city_2 = rand() % currentSolution.size();
        } while (city_1 == city_2);

        std::pair<int, int> neighborhoodCities;

        if (city_1 > city_2) {
            neighborhoodCities.first = currentSolution.at(city_2);
            neighborhoodCities.second = currentSolution.at(city_1);
        } else {
            neighborhoodCities.first = currentSolution.at(city_1);
            neighborhoodCities.second = currentSolution.at(city_2);
        }

        randomCitiesToNeighbourhood.push_back({currentSolution.at(city_1), currentSolution.at(city_2)});
    }

    return randomCitiesToNeighbourhood;
}

std::vector<int>
TabuSearchAlgorithm::GenerateBestNewCurrentSolutionFromNeighbourhood(std::vector<int> currentSolution,
                                                                     std::list<std::pair<int, int>> neighbours,
                                                                     std::pair<int, int> neighborhoodCities,
                                                                     std::string neighborhoodType) {
    std::vector<int> bestNeighbour;

    int bestNeighbourCost = -1;
    int currentNeighbourCost = -1;

    for (auto cities : neighbours) {
        auto tempSolution = currentSolution;

        long city_1 = distance(
                tempSolution.begin(),
                find(tempSolution.begin(), tempSolution.end(), cities.first)
        );

        long city_2 = distance(
                tempSolution.begin(),
                find(tempSolution.begin(), tempSolution.end(), cities.second)
        );

        if (neighborhoodType == "swap")
            neighbourBySwap(tempSolution, city_1, city_2);
        else if (neighborhoodType == "insert")
            neighbourByInsert(tempSolution, city_1, city_2);
        else if (neighborhoodType == "invert")
            neighbourByInvert(tempSolution, city_1, city_2);

        currentNeighbourCost = countCost(tempSolution);

        if ((bestNeighbourCost == -1 || currentNeighbourCost < bestNeighbourCost) &&
            citiesDoNotExistOnTabuList(cities, tempSolution, currentNeighbourCost)) {
            bestNeighbourCost = currentNeighbourCost;
            bestNeighbour = tempSolution;
            neighborhoodCities = cities;
        }
    }

    return bestNeighbour;
}

const std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> &TabuSearchAlgorithm::getIntermediateSolutions() const {
    return intermediateSolutions;
}

void TabuSearchAlgorithm::decrementTenures(std::list<std::pair<std::pair<int, int>, int>> &tabuList) {
    for (auto iterator = tabuList.begin(); iterator != tabuList.end();) {
        if (0 == iterator->second)
            iterator = tabuList.erase(iterator);
        else {
            iterator->second--;
            ++iterator;
        }
    }
}

void TabuSearchAlgorithm::neighbourBySwap(std::vector<int> &tempSolution, long city_1, long city_2) {
    std::swap(tempSolution[city_1], tempSolution[city_2]);
}

void TabuSearchAlgorithm::neighbourByInsert(std::vector<int> &tempSolution, long city_1, long city_2) {
    if (city_1 > city_2) {
        auto temp = tempSolution[city_1];
        for (auto i = city_1; i > city_2; i--)
            tempSolution[i] = tempSolution[i - 1];
        tempSolution[city_2] = temp;
    } else {
        auto temp = tempSolution[city_2];
        for (auto i = city_2; i > city_1; i--)
            tempSolution[i] = tempSolution[i - 1];
        tempSolution[city_1] = temp;
    }
}

void TabuSearchAlgorithm::neighbourByInvert(std::vector<int> &tempSolution, long city_1, long city_2) {
    if (city_1 < city_2) {
        std::reverse(std::begin(tempSolution) + city_1, std::end(tempSolution) - (tempSolution.size() - city_2 - 1));
    } else {
        std::reverse(std::begin(tempSolution) + city_2, std::end(tempSolution) - (tempSolution.size() - city_1 - 1));
    }
}
