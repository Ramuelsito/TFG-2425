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

class SolutionData {
 public:
  SolutionData() {
    best_solution_ = nullptr;
    max_differences_objective_function_ = 0;
    min_differences_objective_function_ = 0;
    mean_differences_objective_function_ = 0;
    max_size_of_walk_ = 0;
    min_size_of_walk_ = 0;
    mean_size_of_walk_ = 0;
  }
 private:
  Solution* best_solution_;
  int max_differences_objective_function_;
  int min_differences_objective_function_;
  int mean_differences_objective_function_;
  int max_size_of_walk_;
  int min_size_of_walk_;
  int mean_size_of_walk_;
};