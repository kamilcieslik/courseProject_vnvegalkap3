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

void TabuSearchAlgorithm::DoCalculations(int tabuListSize, int amountOfNeighbours,
                                         int maximumIterationsWithoutBetterSolution,
                                         int maximumRestarts, int cadence, int maximumRestartsWithoutBetterSolution, bool isIntensification) {
    if (isIntensification) {
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

        int amountOfIterations = 0;
        int amountOfRestarts = 0;
        int iterationsWithoutBetterSolution = 0;
        int restartsWithoutBetterSolution = 0;
        bool foundBetterSolutionAfterRestart = false;
        while (amountOfRestarts < maximumRestarts) {
            std::list<std::pair<int, int>> neighbourhood = findRandomNeighbourhood(currentSolution, amountOfNeighbours);

            currentSolution = aspirationPlus(currentSolution, neighbourhood, replacedCities);
            currentCost = countCost(currentSolution);

            if (currentCost < length) {
                optimalWay = currentSolution;
                length = currentCost;
                intermediateSolutions.push_back(
                        IntermediateSolutionOfTheTabuSearchAlgorithm(timeMeasurement.GetTimeInSeconds(), length));
                std::cout << std::fixed << std::setprecision(6)
                          << intermediateSolutions.back().getTimeFromTheBeginningOfTheAlgorithm() << "\t|\t"
                          << intermediateSolutions.back().getActualResult();
                iterationsWithoutBetterSolution = 0;
                foundBetterSolutionAfterRestart = true;
            } else
                iterationsWithoutBetterSolution++;

            if (tabuList.size() > tabuListSize) {
                tabuList.pop_front();
            }

            std::pair<std::pair<int, int>, int> tabuMovement{replacedCities, cadence};
            tabuList.push_back(tabuMovement);
            decrementCadences(tabuList);

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
    else
    {
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

        int amountOfIterations = 0;
        int amountOfRestarts = 0;
        int iterationsWithoutBetterSolution = 0;
        while (amountOfRestarts < maximumRestarts) {
            std::list<std::pair<int, int>> neighbourhood = findRandomNeighbourhood(currentSolution, amountOfNeighbours);

            currentSolution = aspirationPlus(currentSolution, neighbourhood, replacedCities);
            currentCost = countCost(currentSolution);

            if (currentCost < length) {
                optimalWay = currentSolution;
                length = currentCost;
                intermediateSolutions.push_back(
                        IntermediateSolutionOfTheTabuSearchAlgorithm(timeMeasurement.GetTimeInSeconds(), length));
                iterationsWithoutBetterSolution = 0;
            } else
                iterationsWithoutBetterSolution++;

            if (tabuList.size() > tabuListSize) {
                tabuList.pop_front();
            }

            std::pair<std::pair<int, int>, int> tabuMovement{replacedCities, cadence};
            tabuList.push_back(tabuMovement);
            decrementCadences(tabuList);

            if (CriticalEvent(iterationsWithoutBetterSolution, maximumIterationsWithoutBetterSolution)) {
                Restart(currentSolution, currentCost);
                iterationsWithoutBetterSolution = 0;
                amountOfRestarts++;
            }
            amountOfIterations++;
        }
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
    for (int i = 0; i < amountOfCities; i++) {
        solution.push_back(i);
    }
    std::shuffle(solution.begin(), solution.end(), std::mt19937(std::random_device()()));
    return solution;
}

long long int TabuSearchAlgorithm::countCost(std::vector<int> path) {
    long long int cost = 0;
    for (std::size_t i = 0; i < path.size() - 1; i++) {
        cost += matrixOfCities[path[i]][path[i + 1]];
    }
    cost += matrixOfCities[path[path.size() - 1]][path[0]];

    return cost;
}

bool TabuSearchAlgorithm::checkTabu(std::pair<int, int> &swaped_cities, std::vector<int> &path,
                                    long long int neighbour_cost) {
    auto iterator = tabuList.begin();
    while (iterator != tabuList.end()) {
        if (swaped_cities.first == iterator->first.second ||
            swaped_cities.second == iterator->first.first) {
            long first_element_index =
                    std::find(path.begin(), path.end(), iterator->first.first) - path.begin();
            long second_element_index =
                    std::find(path.begin(), path.end(), iterator->first.second) - path.begin();
            if (first_element_index < second_element_index && neighbour_cost > length)
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
TabuSearchAlgorithm::findRandomNeighbourhood(std::vector<int> &solution, int neighbourhoodSize) {
    std::list<std::pair<int, int>> neighbourhood;

    for (int i = 0; i < neighbourhoodSize; i++) {
        unsigned long firstPosition = rand() % solution.size();

        unsigned long secondPosition;
        do {
            secondPosition = rand() % solution.size();
        } while (firstPosition == secondPosition);

        std::pair<int, int> swaped_cities;

        if (firstPosition > secondPosition) {
            swaped_cities.first = solution.at(secondPosition);
            swaped_cities.second = solution.at(firstPosition);
        } else {
            swaped_cities.first = solution.at(firstPosition);
            swaped_cities.second = solution.at(secondPosition);
        }

        std::pair<int, int> neighbour{solution.at(firstPosition), solution.at(secondPosition)};

        neighbourhood.push_back(neighbour);
    }

    return neighbourhood;
}

std::vector<int>
TabuSearchAlgorithm::aspirationPlus(std::vector<int> solution, std::list<std::pair<int, int>> neighbour,
                                    std::pair<int, int> pair) {
    std::vector<int> best_neighbour;

    int best_neighbour_Q = -1;
    int current_neighbour_Q = -1;

    for (std::pair<int, int> swaped_pair : neighbour) {
        std::vector<int> tempSolution = solution;

        long swap_1 = distance(
                tempSolution.begin(),
                find(tempSolution.begin(), tempSolution.end(), swaped_pair.first)
        );

        long swap_2 = distance(
                tempSolution.begin(),
                find(tempSolution.begin(), tempSolution.end(), swaped_pair.second)
        );

        uint temp = tempSolution[swap_1];
        tempSolution[swap_1] = tempSolution[swap_2];
        tempSolution[swap_2] = temp;

        current_neighbour_Q = countCost(tempSolution);

        if ((best_neighbour_Q == -1 || current_neighbour_Q < best_neighbour_Q) &&
            checkTabu(swaped_pair, tempSolution, current_neighbour_Q)) {
            best_neighbour_Q = current_neighbour_Q;
            best_neighbour = tempSolution;
            pair = swaped_pair;
        }
    }

    return best_neighbour;
}

const std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> &TabuSearchAlgorithm::getIntermediateSolutions() const {
    return intermediateSolutions;
}

void TabuSearchAlgorithm::decrementCadences(std::list<std::pair<std::pair<int, int>, int>> &tabuList) {
    for (auto iterator = tabuList.begin(); iterator != tabuList.end();) {
        if (0 == iterator->second)
            iterator = tabuList.erase(iterator);
        else {
            iterator->second--;
            ++iterator;
        }
    }
}
