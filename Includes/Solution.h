/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Solution class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <fstream>
#include "Machine.h"

/**
 * @class Solution
 * @brief Class that represents a solution
 */
class Solution {
 public:
  Solution() = default;
  Solution(std::vector<Machine> machines) : machines_{machines} {}

  std::vector<Machine> getMachines() const { return machines_; }
  int GetTCT() const {
    int tct = 0;
    for (int i = 0; i < machines_.size(); i++) {
      tct += machines_[i].GetTotalTime();
    }
    return tct;
  }
  void AssignMaxTCT() {
    Machine max_tct_machine = Machine(0, std::vector<Task>(), 999999);
    machines_.push_back(max_tct_machine);
  }
  void PrintStudiedSolution(const std::string& filename, const std::string& algorithm_name, const double& time, const int& number_tasks) {
    std::ofstream file;
    std::string true_filename = "../SolutionTables/SolutionTable" + algorithm_name + ".md";
    file.open(true_filename, std::ios::app);
    file << "| " << filename << " | " << machines_.size() << " | " << number_tasks <<  " | " << GetTCT() << " | " << time << " |" << std::endl; 
    file.close();
  }

  bool operator==(const Solution& solution) const {
    if (machines_.size() != solution.machines_.size()) return false;
    for (int i = 0; i < machines_.size(); i++) {
      if (machines_[i].getTasksAssigned().size() != solution.machines_[i].getTasksAssigned().size()) return false;
      for (int j = 0; j < machines_[i].getTasksAssigned().size(); j++) {
        if (machines_[i].getTasksAssigned()[j] != solution.machines_[i].getTasksAssigned()[j]) return false;
      }
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const Solution& solution) {
    for (int i = 0; i < solution.machines_.size(); i++) {
      os << "Machine " << i << ": ";
      for (int j = 0; j < solution.machines_[i].getTasksAssigned().size(); j++) {
        os << solution.machines_[i].getTasksAssigned()[j] << " ";
      }
      os << std::endl;
    }
    os << "Total completion time: " << solution.GetTCT() << "\n";
    return os;
  }
 private:
  std::vector<Machine> machines_;
};