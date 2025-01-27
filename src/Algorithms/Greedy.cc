/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Greedy algorithm methods implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Algorithms/Greedy.h"

Solution Greedy::Solve() {
  std::vector<Task> tasks_to_assign = problem_.getTasksTimes();
  machines_assigned_ = problem_.getMachines();

  for (int i = 0; i < machines_assigned_.size(); i++) {
    int min_time = 9999999;
    int task_to_assign = -1;
    int machine_to_assign = -1;
    for (int j = 0; j < tasks_to_assign.size(); j++) {
      // std::cout << "Task: " << j << " Time: " << tasks_to_assign[j].GetTime() << " Setup time: " << problem_.getSetupTimes()[0][j + 1] << " Machine: " << i << std::endl;
      int actual_time = tasks_to_assign[j].GetTime() + problem_.getSetupTimes()[0][j + 1];
      if (actual_time < min_time && std::find(tasks_assigned_.begin(), tasks_assigned_.end(), j) == tasks_assigned_.end() && machines_assigned_[i].CheckTotalCompletionTime(actual_time, min_time)) {
        min_time = actual_time;
        task_to_assign = j;
        machine_to_assign = i;
      }
    }
    if (task_to_assign != -1) {
      int task_time = tasks_to_assign[task_to_assign].GetTime() + problem_.getSetupTimes()[0][task_to_assign + 1];
      // std::cout << "Task time: " << task_time << std::endl;
      machines_assigned_[machine_to_assign].AddTask(tasks_to_assign[task_to_assign], task_time);
      tasks_assigned_.push_back(task_to_assign); 
    }
  }
  
  while (tasks_assigned_.size() < tasks_to_assign.size()) {
    int min_time = 9999999;
    int task_to_assign = -1;
    int machine_to_assign = -1;
    FindBestTaskMachinePosition(tasks_to_assign, min_time, task_to_assign, machine_to_assign);
    if (task_to_assign != -1) {
      int task_time = tasks_to_assign[task_to_assign].GetTime() + problem_.getSetupTimes()[machines_assigned_[machine_to_assign].GetLastTask().GetId() + 1][task_to_assign + 1];
      // std::cout << "Task time: " << task_time << std::endl;
      machines_assigned_[machine_to_assign].AddTask(tasks_to_assign[task_to_assign], task_time);
      tasks_assigned_.push_back(task_to_assign); 
    }
  }  
  return Solution(machines_assigned_);
}

void Greedy::FindBestTaskMachinePosition(std::vector<Task>& tasks_to_assign, int& min_time, int& task_to_assign, int& machine_to_assign) {
  for (int i = 0; i < machines_assigned_.size(); i++) {
    int last_task_index = machines_assigned_[i].GetLastTask().GetId();
    for (int j = 0; j < tasks_to_assign.size(); j++) {
      // std::cout << "Task: " << j << " Time: " << tasks_to_assign[j].GetTime() << " Setup time: " << problem_.getSetupTimes()[last_task_index + 1][j + 1] << " Machine: " << i << std::endl;
      int actual_time = tasks_to_assign[j].GetTime() + problem_.getSetupTimes()[last_task_index + 1][j + 1] + machines_assigned_[i].GetTotalTime();
      if (actual_time < min_time && std::find(tasks_assigned_.begin(), tasks_assigned_.end(), j) == tasks_assigned_.end() /*&& machines_assigned_[i].CheckTotalCompletionTime(actual_time, min_time)*/) {
        min_time = actual_time;
        task_to_assign = j;
        machine_to_assign = i;
      }
    }
  }
}
