/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Utilitary functions declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <vector> 
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

/**
 * @brief Function that shows the main menu
 *     and returns the option chosen by the user
 * @return int Option chosen by the user
 */
int AlgorithmMenu() {
  system("clear");
  int option;
  std::cout << "1. Greedy" << std::endl;
  std::cout << "2. GRASP" << std::endl;
  std::cout << "3. Multi-GVNS" << std::endl;
  std::cout << "4. Exit" << std::endl;
  std::cout << "Choose an option: ";
  std::cin >> option;
  return option;
}

void Usage() {
  std::cout << "Usage: ./Parallel-machine [OPTION]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h  Show this help message" << std::endl;
  std::cout << "  -all  Run all instances" << std::endl;
}