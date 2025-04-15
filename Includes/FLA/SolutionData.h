/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief NeighborhoodData class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "../Solution.h"

struct SolutionData {
  Solution* best_solution_ = nullptr;
  int impacts_ = 0;
  int max_differences_objective_function_ = -1;
  int min_differences_objective_function_ = 999999;
  int mean_differences_objective_function_ = 0; // Suma acumulada
  int max_size_of_walk_ = -1;
  int min_size_of_walk_ = 999999;
  int mean_size_of_walk_ = 0; // Suma acumulada

  // Siempre que se actualice la tabla, se actualizan los datos
  void Update(int difference, int size_of_walk) {
    impacts_++;
    max_differences_objective_function_ = max_differences_objective_function_ > difference ? max_differences_objective_function_ : difference;
    min_differences_objective_function_ = min_differences_objective_function_ < difference ? min_differences_objective_function_ : difference;
    mean_differences_objective_function_ += difference; // Acumular la suma

    max_size_of_walk_ = max_size_of_walk_ > size_of_walk ? max_size_of_walk_ : size_of_walk;
    min_size_of_walk_ = min_size_of_walk_ < size_of_walk ? min_size_of_walk_ : size_of_walk;
    mean_size_of_walk_ += size_of_walk; // Acumular la suma
  }

  double GetMeanDifferences() const { return impacts_ > 0 ? static_cast<double>(mean_differences_objective_function_) / impacts_ : 0.0; }
  double GetMeanSizeOfWalk() const { return impacts_ > 0 ? static_cast<double>(mean_size_of_walk_) / impacts_ : 0.0; }
};