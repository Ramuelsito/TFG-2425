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

#include "../../Includes/Algorithms/ConstructionPhase.h"
#include <random>
#include <algorithm>

/**
 * @brief Creación de una solución inicial mediante un algoritmo greedy aleatorizado
 * @return Solución obtenida
 */
Solution ConstructionPhase::ConstructGreedyRandSolution() {
  std::vector<Task> tasks_to_assign = problem_.getTasksTimes();
  machines_assigned_ = problem_.getMachines();
  InitializingMachines(tasks_to_assign);
  std::random_device rd;
  std::mt19937 engine(rd());
  while(tasks_assigned_.size() < tasks_to_assign.size()) {
    std::vector<std::pair<Task, int>> candidates = MakeRandomCandidatesList(tasks_to_assign);
    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    int random_index = dist(engine);
    std::pair<Task, int> selected_task = candidates[random_index];
    int selected_machine_index = selected_task.second;
    int tij = selected_task.first.GetTime() + problem_.getSetupTimeIn(
        machines_assigned_[selected_machine_index].GetLastTask().GetId() + 1, selected_task.first.GetId() + 1);
    machines_assigned_[selected_machine_index].AddTask(selected_task.first, tij);
    tasks_assigned_.push_back(selected_task.first.GetId());
  }
  Solution solution(machines_assigned_);
  machines_assigned_.clear();
  tasks_assigned_.clear();
  return solution;
}

/**
 * @brief Inicialización de las máquinas con una tarea
 * @param tasks_to_assign - Tareas a asignar
 */
void ConstructionPhase::InitializingMachines(const std::vector<Task>& tasks_to_assign) {
  std::vector<Task> tasks_to_assign_init = tasks_to_assign;
  for (int i = 0; i < machines_assigned_.size(); i++) {
    machines_assigned_[i].AddTask(tasks_to_assign_init[0], tasks_to_assign_init[0].GetTime());
    tasks_assigned_.push_back(tasks_to_assign_init[0].GetId());
    tasks_to_assign_init.erase(tasks_to_assign_init.begin());
  }
}

/**
 * @brief Creación de una lista de candidatos aleatorizada
 * @param tasks_to_assign - Tareas a asignar
 * @return Lista de candidatos
 */
std::vector<std::pair<Task, int>> ConstructionPhase::MakeRandomCandidatesList(std::vector<Task>& tasks_candidates) {
  std::vector<Task> tasks_to_assign = tasks_candidates;
  std::vector<std::pair<Task, int>> candidates;
  for (int k = 0; k < 3; k++) {     // Seleccionamos k candidatos aleatorios
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