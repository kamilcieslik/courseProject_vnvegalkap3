//
// Created by mrfarinq on 06.12.17.
//

#include <algorithm>
#include <random>
#include "TabuSearchAlgorithm.h"

TabuSearchAlgorithm::TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(matrixOfCities),
                                                                                     amountOfCities(amountOfCities) {
}

void TabuSearchAlgorithm::DoCalculations(int tabu_list_size, int neighbours_count, int steps_without_change,
                                         int steps_with_random_neighbourhood) {
    if (amountOfCities == 0) return;
    srand(static_cast<unsigned int>(time(nullptr)));

    int current_cost;
    std::vector<int> current_solution(generateRandomSolution());
    current_cost = countCost(current_solution);

    optimalWay = current_solution;
    length = current_cost;

    std::pair<int, int> swaped_cities;
    std::pair<int, int> saved_swap;

    std::vector<int> the_best_neighbour;
    std::vector<int> neighbour;
    int neighbour_cost;
    int the_best_neighbour_cost;
    int steps = 0;
    int change_neighbourhood_count = 0;
    do {
        the_best_neighbour_cost = std::numeric_limits<int>::max();
        for (int i = 0; i < neighbours_count; i++) {
            neighbour = current_solution;
            swaped_cities = swapTwoRandomCities(neighbour);
            neighbour_cost = countCost(neighbour);
            if (neighbour_cost < the_best_neighbour_cost && checkTabu(swaped_cities, neighbour, neighbour_cost)) {
                the_best_neighbour_cost = neighbour_cost;
                the_best_neighbour = neighbour;
                saved_swap = swaped_cities;
            }
        }
        current_solution = the_best_neighbour;
        current_cost = the_best_neighbour_cost;

        if (current_cost < length) {
            optimalWay = current_solution;
            length = current_cost;
            steps = 0;
        }

        if (tabu_list.size() > tabu_list_size) {
            tabu_list.pop_front();
        }
        tabu_list.push_back(saved_swap);
        steps++;
        if (steps > steps_without_change) {
            current_solution = generateRandomSolution();
            current_cost = countCost(current_solution);
            change_neighbourhood_count++;
            steps = 0;
        }
    } while (change_neighbourhood_count < steps_with_random_neighbourhood);
}

std::vector<int> TabuSearchAlgorithm::generateRandomSolution() {
    std::vector<int> solution;
    for (int i = 0; i < amountOfCities; i++) {
        solution.push_back(i);
    }
    std::shuffle(solution.begin(), solution.end(), std::mt19937(std::random_device()()));
    return solution;
}

int TabuSearchAlgorithm::countCost(std::vector<int> path) {
    int cost = 0;
    for (std::size_t i = 0; i < path.size() - 1; i++) {
        cost += matrixOfCities[path[i]][path[i + 1]];
    }
    cost += matrixOfCities[path[path.size() - 1]][path[0]];

    return cost;
}

std::pair<int, int> TabuSearchAlgorithm::swapTwoRandomCities(std::vector<int> &path) {
    unsigned long firstPosition = rand() % path.size();

    unsigned long secondPosition;
    do {
        secondPosition = rand() % path.size();
    } while (firstPosition == secondPosition);

    std::pair<int, int> swaped_cities;

    if (firstPosition > secondPosition) {
        swaped_cities.first = path.at(secondPosition);
        swaped_cities.second = path.at(firstPosition);
    } else {
        swaped_cities.first = path.at(firstPosition);
        swaped_cities.second = path.at(secondPosition);
    }

    int buffor = path.at(firstPosition);
    path.at(firstPosition) = path.at(secondPosition);
    path.at(secondPosition) = buffor;

    return swaped_cities;
}

bool TabuSearchAlgorithm::checkTabu(std::pair<int, int> &swaped_cities, std::vector<int> &path, int neighbour_cost) {
    auto list_iterator = tabu_list.begin();
    while (list_iterator != tabu_list.end()) {
        if (swaped_cities.first == list_iterator->second || swaped_cities.second == list_iterator->first) {
            long first_element_index = std::find(path.begin(), path.end(), list_iterator->first) - path.begin();
            long second_element_index = std::find(path.begin(), path.end(), list_iterator->second) - path.begin();
            if (first_element_index < second_element_index && neighbour_cost > length)
                return false;
        }
        list_iterator++;
    }
    return true;
}

std::pair<std::vector<int>, int> TabuSearchAlgorithm::GetResults() {
    return std::make_pair(optimalWay, length);
}