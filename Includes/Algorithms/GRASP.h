/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief GRASP interface declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "../Algorithm.h"

/**
 * @class GRASP
 * @brief Class that represents the GRASP algorithm
 */
class GRASP : public Algorithm {
 public: 
  GRASP(Problem problem) : problem_{problem} {
    machines_assigned_.clear();
    tasks_assigned_.clear();
  }
  GRASP(Problem problem, std::vector<Machine> machines) : problem_{problem}, machines_assigned_{machines} {}

  Solution Solve() override;
  virtual Solution ConstructGreedyRandSolution() = 0;
  virtual Solution LocalSearch(const Solution&) = 0;
  virtual Solution UpdateSolution(const Solution&, const Solution&) = 0;
 protected:
  Problem problem_;
  std::vector<Machine> machines_assigned_;
  std::vector<int> tasks_assigned_;
};