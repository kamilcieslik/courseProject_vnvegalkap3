#include <iostream>
#include "TimeMeasurement.h"
#include "TSPLIB_Parser.h"
#include "TravellingSalesmanProblem.h"
#include "AlgorithmTest.h"

void displayMenu(const std::string &info) //Menu dla problemu komiwojażera.
{
    std::cout << std::endl;
    std::cout << info << std::endl;
    std::cout << "1. Wczytaj z pliku TSPLIB." << std::endl;
    std::cout << "2. Wczytaj z pliku TXT." << std::endl;
    std::cout << "3. Wygeneruj miasta losowo." << std::endl;
    std::cout << "4. Wyświetl informacje o danych wejściowych problemu." << std::endl;
    std::cout << "5. Algorytm - przeszukiwanie zupełne." << std::endl;
    std::cout << "6. Algorytm - metoda podziału i ograniczeń." << std::endl;
    std::cout << "7. Algorytm - algorytm z zakazami." << std::endl;
    std::cout << "0. Powrót." << std::endl;
    std::cout << "Podaj opcje: ";
}

void menu_tabu_search_neighborhoodType_menu(TravellingSalesmanProblem &s){
    int option;
    do {
        std::cout << std::endl;
        std::cout << "*** Rodzaj sąsiedztwa Tabu Search ***" << std::endl;
        std::cout << "1. SWAP." << std::endl;
        std::cout << "2. INSERT." << std::endl;
        std::cout << "2. INVERT." << std::endl;
        std::cout << "0. Powrót." << std::endl;
        std::cout << "Podaj opcje: ";
        std::cin >> option;
        std::cout << std::endl;
        switch (option) {
            case 1: // Metoda generowania sąsiedztwa typu SWAP.
                try {
                    TimeMeasurement t;
                    t.TimeStart();
                    s.PerformTabuSearchAlgorithm("swap");
                    t.TimeStop();
                    s.PrintCitiesForTheTravellingSalesman(false);
                    std::cout << std::endl;
                    s.PrintSolution();
                    std::cout.setf(std::ios::fixed, std::ios::floatfield);
                    std::cout.setf(std::ios::showpoint);
                    std::cout << "Time\t= " << t.GetTimeInSeconds() << " s" << std::endl << std::endl;
                }
                catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 2: // Metoda generowania sąsiedztwa typu INSERT.
                try {
                    TimeMeasurement t;
                    t.TimeStart();
                    s.PerformTabuSearchAlgorithm("insert");
                    t.TimeStop();
                    s.PrintCitiesForTheTravellingSalesman(false);
                    std::cout << std::endl;
                    s.PrintSolution();
                    std::cout.setf(std::ios::fixed, std::ios::floatfield);
                    std::cout.setf(std::ios::showpoint);
                    std::cout << "Time\t= " << t.GetTimeInSeconds() << " s" << std::endl << std::endl;
                }
                catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 3: // Metoda generowania sąsiedztwa typu INVERT.
                try {
                    TimeMeasurement t;
                    t.TimeStart();
                    s.PerformTabuSearchAlgorithm("invert");
                    t.TimeStop();
                    s.PrintCitiesForTheTravellingSalesman(false);
                    std::cout << std::endl;
                    s.PrintSolution();
                    std::cout.setf(std::ios::fixed, std::ios::floatfield);
                    std::cout.setf(std::ios::showpoint);
                    std::cout << "Time\t= " << t.GetTimeInSeconds() << " s" << std::endl << std::endl;
                }
                catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            default:
                break;
        }

    } while (option != 0);
}

void menu_travelling_salesman_problem() //Obsługa problemu komiwojażera.
{
    TravellingSalesmanProblem s;
    std::string path;
    int option;
    do {
        displayMenu("*** Problem komiwojażera ***");
        std::cin >> option;
        std::cout << std::endl;
        switch (option) {
            case 1: //Tworzenie zbioru miast z pliku TSPLIB.
                std::cout << "Podaj sciezke pliku z danymi TSPLIB: ";
                std::cin >> path;
                try {
                    TSPLIB_Parser parser(path);
                    s.LoadArrayOfMatrixOfCities(parser.GetArrayOfMatrixCities(), parser.GetDimension(),
                                                parser.GetFileName(), parser.GetGraphType());
                } catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 2: //Tworzenie zbioru miast ze standardowego pliku txt.
                std::cout << "Podaj sciezke pliku z danymi: ";
                std::cin >> path;
                try {
                    s.ReadCitiesFromNormalFile(path);
                } catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 3: //Generowanie miast pseudolosowo.
                try {
                    s.GenerateRandomCities();
                }
                catch (std::invalid_argument &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 4: //Wyświetlanie zbioru miast.
                try {
                    s.PrintCitiesForTheTravellingSalesman(true);
                }
                catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 5: //Algorytm 1. - przegląd zupełny.
                try {
                    TimeMeasurement t;
                    t.TimeStart();
                    s.PerformBruteForceAlgorithm();
                    t.TimeStop();
                    s.PrintCitiesForTheTravellingSalesman(false);
                    std::cout << std::endl;
                    s.PrintSolution();
                    std::cout.setf(std::ios::fixed, std::ios::floatfield);
                    std::cout.setf(std::ios::showpoint);
                    std::cout << "Time\t= " << t.GetTimeInSeconds() << " s" << std::endl << std::endl;
                }
                catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 6: //Algorytm 2. - metoda podziału i ograniczeń.
                try {
                    TimeMeasurement t;
                    t.TimeStart();
                    s.PerformBranchAndBoundAlgorithm();
                    t.TimeStop();
                    s.PrintCitiesForTheTravellingSalesman(false);
                    std::cout << std::endl;
                    s.PrintSolution();
                    std::cout.setf(std::ios::fixed, std::ios::floatfield);
                    std::cout.setf(std::ios::showpoint);
                    std::cout << "Time\t= " << t.GetTimeInSeconds() << " s" << std::endl << std::endl;
                }
                catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 7: //Algorytm 3. - algorytm z zakazami.
                menu_tabu_search_neighborhoodType_menu(s);
                break;

            default:
                break;
        }

    } while (option != 0);
}



void menu_tests() //Obsługa testów czasowych.
{
    AlgorithmTest test;
    int option;
    int numberOfRepetitions;
    do {
        std::cout << std::endl;
        std::cout << "*** Testy czasowe ***" << std::endl;
        std::cout << "1. Testy czasowe dla algorytmu przeglądu zupełnego problemu komiwojażera." << std::endl;
        std::cout << "2. Testy czasowe dla algorytmu podziału i ograniczeń problemu komiwojażera." << std::endl;
        std::cout << "0. Powrót do menu." << std::endl;
        std::cout << "Podaj opcje: ";
        std::cin >> option;
        std::cout << std::endl;
        switch (option) {
            case 1: //Testy czasowe dla algorytmu przeglądu zupełnego problemu komiwojażera.
                std::cout << "Podaj ilość instancji każdego zestawu danych w celu uśrednienia wyniku: ";
                std::cin >> numberOfRepetitions;
                test.TravellingSalesmanProblem_Test_BruteForce(numberOfRepetitions);
                break;

            case 2: //Testy czasowe dla algorytmu podziału i ograniczeń problemu komiwojażera.
                std::cout << "Podaj ilość instancji każdego zestawu danych w celu uśrednienia wyniku: ";
                std::cin >> numberOfRepetitions;
                test.TravellingSalesmanProblem_Test_BranchAndBound(numberOfRepetitions);
                break;

            default:
                break;
        }

    } while (option != 0);
}

int main() {
    int option;
    do {
        std::cout << std::endl;
        std::cout << "==== MENU GŁÓWNE ===" << std::endl;
        std::cout << "1. Problem komiwojażera." << std::endl;
        std::cout << "2. Testy czasowe." << std::endl;
        std::cout << "0. Wyjście." << std::endl;
        std::cout << "Podaj opcje: ";
        std::cin >> option;
        std::cout << std::endl;

        switch (option) {
            case 1:
                menu_travelling_salesman_problem();
                break;
            case 2:
                menu_tests();
                break;
            default:
                break;
        }

    } while (option != 0);

    return 0;
}
