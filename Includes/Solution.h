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
  Solution(std::vector<Machine> machines) : machines_{machines} { total_completion_time_ = 0; }

  std::vector<Machine> getMachines() const { return machines_; }
  int GetTCT() const { return total_completion_time_; }
  void AddTCT(const int& tct) { total_completion_time_ += tct; }
  
  void RecalculateTotalCompletionTime();
  void InsertTask(const Task& task, const int& machine_id, const int& task_position, const int& tct_increment = 0);
  void RemoveTask(const int& machine_id, const int& task_index, const int& tct_decrement = 0);
  void ExchangeTasks(const int& machine_id, const int& i, const int& j) { machines_[machine_id].ExchangeTasks(i, j); }
  int EmulateSwapIntra(const int& machine_id, const int& task_index1, const int& task_index2) const;
  int EmulateSwapInter(const int& machine_id1, const int& task_index1, const int& machine_id2, const int& task_index2) const;
  int EmulateReInsertion(const int& machine_id, const int& task_index) const;
  int CalculateDistance(const Solution& solution) const;
  void PrintStudiedSolution(const std::string& filename, const std::string& algorithm_name, const double& time, const int& number_tasks);
  bool operator==(const Solution& solution) const;
  const Machine& operator[](int i) const { return machines_[i]; }

  friend std::ostream& operator<<(std::ostream& os, const Solution& solution) {
    for (int i = 0; i < solution.machines_.size(); i++) {
      os << "Machine " << i << ": taks assigned: " << solution.machines_[i].size() << "\n";
      for (int j = 0; j < solution.machines_[i].size(); j++) {
        os << solution.machines_[i][j] << " ";
      }
      os << std::endl;
    }
    os << "Total completion time: " << solution.total_completion_time_ << "\n";
    return os;
  }
 private:
  std::vector<Machine> machines_;
  int total_completion_time_;
};