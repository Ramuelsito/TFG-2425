/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Studied solution class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "SolutionData.h"
#include "NeighborhoodData.h"
#include "../Instances/InstanceData.h"

class StudiedSolution {
 public:
  StudiedSolution() : best_solution_(nullptr), solution_tct_(0), number_hits_(0), differences_objective_function_(0), max_differences_objective_function_(0), min_differences_objective_function_(0) {}
  StudiedSolution(Solution* best_solution, int solution_tct, int number_hits, int differences_objective_function, int max_differences_objective_function, int min_differences_objective_function) :
    best_solution_(best_solution), solution_tct_(solution_tct), number_hits_(number_hits), differences_objective_function_(differences_objective_function),
    max_differences_objective_function_(max_differences_objective_function), min_differences_objective_function_(min_differences_objective_function) {}
 private:
  Solution* best_solution_;
  int solution_tct_;
  int number_hits_;
  int differences_objective_function_;
  int max_differences_objective_function_;
  int min_differences_objective_function_;
};