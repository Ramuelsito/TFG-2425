/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Fase constructiva del algoritmo Multi-GVNS
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once 
#include "../Problem.h"
#include "../Solution.h"

class ConstructionPhase {
 public:
  ConstructionPhase(const Problem& problem) : problem_(problem) {};
  Solution ConstructGreedyRandSolution();
 private:
  void InitializingMachines(const std::vector<Task>& tasks_to_assign);
  std::vector<std::pair<Task, int>> MakeRandomCandidatesList(std::vector<Task>& tasks_to_assign);

  const Problem& problem_;
  std::vector<Machine> machines_assigned_;
  std::vector<int> tasks_assigned_;
};
