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
#include "../Includes/Problem.h"

void Solution::RecalculateTotalCompletionTime() {
  total_completion_time_ = 0;
  for (int i = 0; i < machines_.size(); i++) {
    total_completion_time_ += machines_[i].RecalculateTotalCompletionTime();
  }
}

/**
 * @brief Emulate the swap intra movement, without modifying the solution
 * @param machine_id - Machine id
 * @param i - Task index 1
 * @param j - Task index 2
 * @return Delta TCT
 */
int Solution::EmulateSwapIntra(const int& machine_id, const int& i, const int& j) const {
  int tasks_assigned = machines_[machine_id].size();
  int new_tij1 = 0;
  int old_tij1 = 0;
  if (i == 0) {
    old_tij1 = machines_[machine_id][i].GetTime() + Problem::getInstance().CalculateSij(0, machines_[machine_id][i].GetId() + 1);
    new_tij1 = machines_[machine_id][j].GetTime() + Problem::getInstance().CalculateSij(0, machines_[machine_id][j].GetId() + 1);
  } else {
    old_tij1 = machines_[machine_id][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][i - 1].GetId() + 1, machines_[machine_id][i].GetId() + 1);
    new_tij1 = machines_[machine_id][j].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][i - 1].GetId() + 1, machines_[machine_id][j].GetId() + 1);
  }
  int new_tij2 = 0; 
  int old_tij2 = 0; 
  if (i == tasks_assigned - 1) {
    new_tij2 = machines_[machine_id][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][j].GetId() + 1, machines_[machine_id][i].GetId() + 1);
    old_tij2 = machines_[machine_id][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][i - 1].GetId() + 1, machines_[machine_id][i].GetId() + 1);
  } else {
    new_tij2 = machines_[machine_id][i + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][j].GetId() + 1, machines_[machine_id][i + 1].GetId() + 1);
    old_tij2 = machines_[machine_id][i + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][i].GetId() + 1, machines_[machine_id][i + 1].GetId() + 1);
  }
  int new_tij3 = machines_[machine_id][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][j - 1].GetId() + 1, machines_[machine_id][i].GetId() + 1);
  int old_tij3 = machines_[machine_id][j].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][j - 1].GetId() + 1, machines_[machine_id][j].GetId() + 1);
  // std::cout << "Va a petar, numero de tareas: " << tasks_assigned << " j: " << j << std::endl;
  int new_tij4 = 0;
  int old_tij4 = 0;
  if (j >= tasks_assigned - 1) {
    new_tij4 = machines_[machine_id][tasks_assigned - 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][i].GetId() + 1, machines_[machine_id][tasks_assigned - 1].GetId() + 1);
    old_tij4 = machines_[machine_id][tasks_assigned - 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][tasks_assigned - 2].GetId() + 1, machines_[machine_id][tasks_assigned - 1].GetId() + 1);
  } else {
    new_tij4 = machines_[machine_id][j + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][i].GetId() + 1, machines_[machine_id][j + 1].GetId() + 1);
    old_tij4 = machines_[machine_id][j + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id][j].GetId() + 1, machines_[machine_id][j + 1].GetId() + 1);
  }
  // std::cout << "No peto" << std::endl; 
  int delta_tct = (tasks_assigned - (i + 1) + 1) * (new_tij1 - old_tij1) + (tasks_assigned - (i + 1)) * (new_tij2 - old_tij2) + (tasks_assigned - (j + 1) + 1) * (new_tij3 - old_tij3) + (tasks_assigned - (j + 1)) * (new_tij4 - old_tij4);
  return delta_tct;
}

/**
 * @brief Emulate the swap inter movement, without modifying the solution 
 * @param machine_id1 - Machine id associated to task 1
 * @param i - Task index 1
 * @param machine_id2 - Machine id associated to task 2
 * @param j - Task index 2
 * @return Delta TCT
 */
int Solution::EmulateSwapInter(const int& machine_id1, const int& i, const int& machine_id2, const int& j) const {
  int first_machine_size = machines_[machine_id1].size();
  int second_machine_size = machines_[machine_id2].size();
  int new_tij1 = 0;
  int old_tij1 = 0;
  if (i == 0) {
    new_tij1 = machines_[machine_id2][j].GetTime() + Problem::getInstance().CalculateSij(0, machines_[machine_id2][j].GetId() + 1);
    old_tij1 = machines_[machine_id1][i].GetTime() + Problem::getInstance().CalculateSij(0, machines_[machine_id1][i].GetId() + 1);
  } else {
    new_tij1 = machines_[machine_id2][j].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id1][i - 1].GetId() + 1, machines_[machine_id2][j].GetId() + 1);
    old_tij1 = machines_[machine_id1][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id1][i - 1].GetId() + 1, machines_[machine_id1][i].GetId() + 1);
  }
  int new_tij2 = 0; 
  int old_tij2 = 0; 
  if (i == first_machine_size - 1) {
    new_tij2 = machines_[machine_id1][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id2][j].GetId() + 1, machines_[machine_id1][i].GetId() + 1);
    old_tij2 = machines_[machine_id1][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id1][i - 1].GetId() + 1, machines_[machine_id1][i].GetId() + 1);
  } else {
    new_tij2 = machines_[machine_id1][i + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id2][j].GetId() + 1, machines_[machine_id1][i + 1].GetId() + 1);
    old_tij2 = machines_[machine_id1][i + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id1][i].GetId() + 1, machines_[machine_id1][i + 1].GetId() + 1);
  }
  int new_tij3 = 0;
  int old_tij3 = 0;
  if (j == 0){
    new_tij3 = machines_[machine_id1][i].GetTime() + Problem::getInstance().CalculateSij(0, machines_[machine_id1][i].GetId() + 1);
    old_tij3 = machines_[machine_id2][j].GetTime() + Problem::getInstance().CalculateSij(0, machines_[machine_id2][j].GetId() + 1);
  } else {
    new_tij3 = machines_[machine_id1][i].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id2][j - 1].GetId() + 1, machines_[machine_id1][i].GetId() + 1);
    old_tij3 = machines_[machine_id2][j].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id2][j - 1].GetId() + 1, machines_[machine_id2][j].GetId() + 1);
  }
  int new_tij4 = 0;
  int old_tij4 = 0;
  if (j == second_machine_size - 1) {
    new_tij4 = machines_[machine_id2][j].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id1][i].GetId() + 1, machines_[machine_id2][j].GetId() + 1);
    old_tij4 = machines_[machine_id2][j].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id2][j - 1].GetId() + 1, machines_[machine_id2][j].GetId() + 1);
  } else {
    new_tij4 = machines_[machine_id2][j + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id1][i].GetId() + 1, machines_[machine_id2][j + 1].GetId() + 1);
    old_tij4 = machines_[machine_id2][j + 1].GetTime() + Problem::getInstance().CalculateSij(machines_[machine_id2][j].GetId() + 1, machines_[machine_id2][j + 1].GetId() + 1);
  }
  int delta_tct = (first_machine_size - (i + 1) + 1) * (new_tij1 - old_tij1) + (first_machine_size - (i + 1)) * (new_tij2 - old_tij2) + (second_machine_size - (j + 1) + 1) * (new_tij3 - old_tij3) + (second_machine_size - (j + 1)) * (new_tij4 - old_tij4);
  return delta_tct;
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