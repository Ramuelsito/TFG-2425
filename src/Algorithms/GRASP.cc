/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief GRASP algorithm methods implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarsez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Algorithms/GRASP.h"

/**
 * @brief Solve the problem using the GRASP algorithm
 * @return Solution - The solution to the problem
 */
Solution GRASP::Solve() {
  Solution best_solution;
  best_solution.AssignMaxTCT();
  Solution current_solution;
  for (int i = 0; i < 100; ++i) {
    current_solution = ConstructGreedyRandSolution();
    // std::cout << "Current solution: " << std::endl << current_solution << std::endl;
    current_solution = LocalSearch(current_solution);
    // std::cout << "Local search solution: " << std::endl << current_solution << std::endl;
    best_solution = UpdateSolution(current_solution, best_solution);
  }
  return best_solution;
}