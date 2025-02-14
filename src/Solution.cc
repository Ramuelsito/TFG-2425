/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Solution methods implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../Includes/Solution.h"

void Solution::RecalculateTotalCompletionTime() {
  total_completion_time_ = 0;
  for (int i = 0; i < machines_.size(); i++) {
    total_completion_time_ += machines_[i].RecalculateTotalCompletionTime();
  }
}

void Solution::InsertTask(const Task& task, const int& machine_id, const int& task_position, const int& tct_increment) {
  machines_[machine_id].InsertTask(task, task_position, tct_increment);
  total_completion_time_ += tct_increment;
}

void Solution::RemoveTask(const int& machine_id, const int& task_index, const int& tct_decrement) {
  machines_[machine_id].RemoveTask(task_index, tct_decrement);
  total_completion_time_ -= tct_decrement;
}

void Solution::PrintStudiedSolution(const std::string& filename, const std::string& algorithm_name, const double& time, const int& number_tasks) {
  std::ofstream file;
  std::string true_filename = "../SolutionTables/SolutionTable" + algorithm_name + ".md";
  file.open(true_filename, std::ios::app);
  file << "| " << filename << " | " << machines_.size() << " | " << number_tasks <<  " | " << GetTCT() << " | " << time << " |" << std::endl; 
  file.close();
}

bool Solution::operator==(const Solution& solution) const {
  if (machines_.size() != solution.machines_.size()) return false;
  for (int i = 0; i < machines_.size(); i++) {
    if (machines_[i].getTasksAssigned().size() != solution.machines_[i].getTasksAssigned().size()) return false;
    for (int j = 0; j < machines_[i].getTasksAssigned().size(); j++) {
      if (machines_[i].getTasksAssigned()[j] != solution.machines_[i].getTasksAssigned()[j]) return false;
    }
  }
  return true;
}