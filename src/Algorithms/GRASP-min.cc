/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief GRASP-min algorithm methods implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */


#include "../../Includes/Algorithms/GRASP-Algorithms/GRASP-min.h"

#include <random>
#include <algorithm>


bool GRASPMin::IsTaskAssigned(const int& task_id) {
  if (std::find(tasks_assigned_.begin(), tasks_assigned_.end(), task_id) != tasks_assigned_.end()) {
    return true;
  }
  return false;
}

void GRASPMin::InitializingMachines(std::vector<Task>& tasks_to_assign) {
  machines_assigned_ = problem_.getMachines();

  for (int i = 0; i < machines_assigned_.size(); i++) {
    int min_time = 9999999;
    int task_to_assign = -1;
    int machine_to_assign = -1;
    for (int j = 0; j < tasks_to_assign.size(); j++) {
      int actual_time = tasks_to_assign[j].GetTime() + problem_.getSetupTimes()[0][j + 1];
      if (actual_time < min_time && std::find(tasks_assigned_.begin(), tasks_assigned_.end(), j) == tasks_assigned_.end() && machines_assigned_[i].CheckTotalCompletionTime(actual_time, min_time)) {
        min_time = actual_time;
        task_to_assign = j;
        machine_to_assign = i;
      }
    }
    if (task_to_assign != -1) {
      int task_time = tasks_to_assign[task_to_assign].GetTime() + problem_.getSetupTimes()[0][task_to_assign + 1];
      machines_assigned_[machine_to_assign].AddTask(tasks_to_assign[task_to_assign], task_time);
      tasks_assigned_.push_back(task_to_assign); 
    }
  }
}

std::pair<Task, int> GRASPMin::SelectRandomTask(const std::vector<std::pair<Task, int>>& tasks_candidates) {
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<> dist(0, tasks_candidates.size() - 1);
  int random_index = dist(engine);
  return tasks_candidates[random_index];
}

void GRASPMin::AssignTaskToMachine(const Task& task, const int& machine_index) {
  machines_assigned_[machine_index].AddTask(task, (task.GetTime() + problem_.getSetupTimes()[machines_assigned_[machine_index].GetLastTask().GetId() + 1][task.GetId() + 1]));
  tasks_assigned_.push_back(task.GetId());
}

std::vector<std::pair<Task, int>> GRASPMin::MakeRandomCandidatesList(std::vector<Task>& tasks_candidates) {
  std::vector<Task> tasks_to_assign = tasks_candidates;
  std::vector<std::pair<Task, int>> candidates;
  for (int k = 0; k < 3; k++) {
    int min_time = 9999999;
    int task_index = -1;
    int machine_to_assign = -1;
    int aux_index = -1;
    for (int i = 0; i < machines_assigned_.size(); i++) {
      int last_task_index = machines_assigned_[i].GetLastTask().GetId();
      for (int j = 0; j < tasks_to_assign.size(); j++) {
        int actual_time = tasks_to_assign[j].GetTime() + problem_.getSetupTimes()[last_task_index + 1][tasks_to_assign[j].GetId() + 1] + machines_assigned_[i].GetTotalTime();
        if (actual_time < min_time && std::find(tasks_assigned_.begin(), tasks_assigned_.end(), tasks_to_assign[j].GetId()) == tasks_assigned_.end()) {
          min_time = actual_time;
          task_index = j;
          machine_to_assign = i;
        }
      }
    }
    if (task_index != -1) {
      candidates.push_back(std::make_pair(tasks_to_assign[task_index], machine_to_assign));
      tasks_to_assign.erase(std::remove(tasks_to_assign.begin(), tasks_to_assign.end(), tasks_to_assign[task_index]), tasks_to_assign.end());
    }
  }
  return candidates;
}

Solution GRASPMin::ConstructGreedyRandSolution() {
  std::vector<Task> tasks_to_assign = problem_.getTasksTimes();
  machines_assigned_ = problem_.getMachines();
  tasks_assigned_.clear();
  InitializingMachines(tasks_to_assign);
  while (tasks_assigned_.size() < tasks_to_assign.size()) {
    std::vector<std::pair<Task, int>> candidates = MakeRandomCandidatesList(tasks_to_assign);
    std::pair<Task, int> selected_task = SelectRandomTask(candidates);
    AssignTaskToMachine(selected_task.first, selected_task.second);
  }
  return Solution(machines_assigned_);
}

Solution GRASPMin::LocalSearch(const Solution& initial_solution) {
  ReInsertionInter reinsertion_inter = ReInsertionInter(initial_solution, problem_);
  Solution reinsertion_inter_solution = reinsertion_inter.GenerateEnvironment();
  if (reinsertion_inter_solution.GetTCT() < initial_solution.GetTCT()) {
    return reinsertion_inter_solution;
  }
  SwapIntra swap_intra = SwapIntra(initial_solution, problem_);
  Solution swap_intra_solution = swap_intra.GenerateEnvironment();
  if (swap_intra_solution.GetTCT() < initial_solution.GetTCT()) {
    return swap_intra_solution;
  }
  ReInsertionIntra reinsertion_intra = ReInsertionIntra(initial_solution, problem_);
  Solution reinsertion_intra_solution = reinsertion_intra.GenerateEnvironment();
  if (reinsertion_intra_solution.GetTCT() < initial_solution.GetTCT()) {
    return reinsertion_intra_solution;
  }
  SwapInter swap_inter = SwapInter(initial_solution, problem_);
  Solution swap_inter_solution = swap_inter.GenerateEnvironment();
  if (swap_inter_solution.GetTCT() < initial_solution.GetTCT()) {
    return swap_inter_solution;
  }
  return initial_solution;
}

Solution GRASPMin::UpdateSolution(const Solution& actual_solution, const Solution& best_solution) {
  if (actual_solution.GetTCT() < best_solution.GetTCT()) { return actual_solution; }
  return best_solution;
}