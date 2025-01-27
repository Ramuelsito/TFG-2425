/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Greedy algorithm class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "../Algorithm.h"

/**
 * @class Greedy
 * @brief Class that represents the Greedy algorithm
 */
class Greedy : public Algorithm {
 public: 
  Greedy(Problem problem) : problem_(problem) {
    machines_assigned_.clear();
  }
  Greedy(Problem problem, std::vector<Machine> machines ) : problem_(problem), machines_assigned_{machines} {}
  Solution Solve() override;
 private:
  void FindBestTaskMachinePosition(std::vector<Task>&, int&, int&, int&);

  Problem problem_;
  std::vector<Machine> machines_assigned_;
  std::vector<int> tasks_assigned_;
};