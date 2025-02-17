/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Fase constructiva del algoritmo Multi-GVNS, que evalua la 
 *        mejor tarea relación máquina-tarea-posición 
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once 
#include "../Problem.h"
#include "../Solution.h"
#include "../Insertion.h"

class ExhaustedConstructionPhase {
 public:
  ExhaustedConstructionPhase() {
    problem_ = &Problem::getInstance();
    final_solution_ = Solution();
  };
  Solution ConstructGreedyRandSolution();
  void IncreaseRCLSize() { RCL_size_++; }
  void IncreaseAlpha() { alpha_ += 0.1; }
 private:
  void InitializingMachines(std::vector<Task>& tasks_to_assign);
  std::vector<Insertion> MakeRandomCandidatesList(const std::vector<Task>& tasks_to_assign, int machine_index);
  std::vector<Insertion> MakeRandomCandidatesListWithRange(const std::vector<Task>& tasks_candidates, int chosen_machine_index);
  Problem* problem_;
  Solution final_solution_;

  float alpha_ = 0.1;
  int RCL_size_ = 2;
};