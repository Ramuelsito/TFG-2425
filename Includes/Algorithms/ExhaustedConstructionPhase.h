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
    machines_assigned_.clear();
    tasks_assigned_.clear();
  };
  Solution ConstructGreedyRandSolution();
  void IncreaseRCLSize() { RCL_size_++; }
 private:
  void InitializingMachines(std::vector<Task>& tasks_to_assign);
  std::vector<Insertion> MakeRandomCandidatesList(const std::vector<Task>& tasks_to_assign, int machine_index);

  std::vector<Machine> machines_assigned_;
  std::vector<int> tasks_assigned_;
  int RCL_size_ = 2;
};