/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief GRASP-min algorithm class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "../GRASP.h"
#include "../../Environment/ReInsertionIntra.h"
#include "../../Environment/SwapIntra.h"
#include "../../Environment/ReInsertionInter.h"
#include "../../Environment/SwapInter.h"

/**
 * @class GRASPMin
 * Represents the GRASP algorithm 
 * with the objective of minimizing the TCT
 */
class GRASPMin : public GRASP {
 public:
  GRASPMin(Problem problem) : GRASP(problem) {}
  GRASPMin(Problem problem, std::vector<Machine> machines) : GRASP(problem, machines) {}

  Solution ConstructGreedyRandSolution() override;
  Solution LocalSearch(const Solution&) override;
  Solution UpdateSolution(const Solution&, const Solution&) override;
 private: 
  void InitializingMachines(std::vector<Task>&);
  std::vector<std::pair<Task, int>> MakeRandomCandidatesList(std::vector<Task>&);
  std::pair<Task, int> SelectRandomTask(const std::vector<std::pair<Task, int>>&);
  void AssignTaskToMachine(const Task&, const int&);
  bool IsTaskAssigned(const int&);
};