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

#include "../../Includes/Algorithms/ExhaustedConstructionPhase.h"
#include <random>
#include <algorithm>

/**
 * @brief Construcción de una solución inicial mediante un algoritmo greedy con cierta aleatoriedad
 * @return Solución obtenida
 */
Solution ExhaustedConstructionPhase::ConstructGreedyRandSolution() {
  std::cout << "Constructing solution..." << std::endl;
  std::vector<Task> tasks_to_assign = Problem::getInstance().getTasksTimes();
  std::cout << "Tasks to assign: " << tasks_to_assign.size() << std::endl;
  machines_assigned_ = Problem::getInstance().getMachines();
  std::vector<std::vector<int>> setup_times = Problem::getInstance().getSetupTimes();
  std::cout << "Machines: " << machines_assigned_.size() << std::endl;
  std::cout << "Setup times: " << setup_times.size() << std::endl;
  InitializingMachines(tasks_to_assign);
  std::random_device rd;
  std::mt19937 engine(rd());
  while(tasks_assigned_.size() < tasks_to_assign.size()) {
    int machine_index_less_span = 0;
    int less_span = 999999;
    for (int i = 0; i < machines_assigned_.size(); i++) {
      int span = machines_assigned_[i].GetTotalTime();
      if (span < less_span) {
        less_span = span;
        machine_index_less_span = i;
      } 
    }
    std::vector<Insertion> candidates = MakeRandomCandidatesList(tasks_to_assign, machine_index_less_span);
    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    int random_index = dist(engine);
    Insertion selected_task = candidates[random_index];
    machines_assigned_[machine_index_less_span].InsertTask(selected_task.task, selected_task.dest_task_index, selected_task.increment_tct);
    tasks_assigned_.push_back(selected_task.task.GetId());
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
void ExhaustedConstructionPhase::InitializingMachines(const std::vector<Task>& tasks_to_assign) {
  std::vector<Task> tasks_to_assign_init = tasks_to_assign;
  for (int i = 0; i < machines_assigned_.size(); i++) {
    machines_assigned_[i].InsertTask(tasks_to_assign_init[0], 0, tasks_to_assign_init[0].GetTime());
    tasks_assigned_.push_back(tasks_to_assign_init[0].GetId());
    tasks_to_assign_init.erase(tasks_to_assign_init.begin());
  }
}

/**
 * @brief Creación de una lista de candidatos aleatorizada
 * @param tasks_to_assign - Tareas a asignar
 * @return Lista de candidatos
 */
std::vector<Insertion> ExhaustedConstructionPhase::MakeRandomCandidatesList(const std::vector<Task>& tasks_candidates, int chosen_machine_index) {
  std::vector<Task> tasks_in_machine = machines_assigned_[chosen_machine_index].getTasksAssigned();
  std::vector<Task> current_candidates = tasks_candidates;
  std::vector<Insertion> candidates;
  int best_tct_increment = 999999;
  int best_task_position = 0;
  int tasks_assigned_size = tasks_in_machine.size();
  for (int k = 0; k < RCL_size_; k++) {
    for (int q = 0; q < tasks_in_machine.size(); q++) {       // q Es la posición de la tarea en la máquina
      for (int i = 0; i < current_candidates.size(); i++) {
        int tct_increment = 0;
        if (q == 0) {
          int new_tij = tasks_in_machine[0].GetTime() + Problem::getInstance().CalculateSij(current_candidates[i].GetId() + 1, tasks_in_machine[0].GetId() + 1);
          int old_first_task_tij = tasks_in_machine[0].GetTime();
          tct_increment = (tasks_assigned_size + 1) * current_candidates[i].GetTime() + 
                           tasks_assigned_size * (new_tij - old_first_task_tij);
        } else if(1 <= q <= tasks_assigned_size) {
          for (int l = 1; l < q - 1; l++) {
            int other_tij = tasks_in_machine[l].GetTime() + Problem::getInstance().CalculateSij(current_candidates[l - 1].GetId() + 1, tasks_in_machine[l].GetId() + 1);
            int new_tij = current_candidates[i].GetTime() + Problem::getInstance().CalculateSij(tasks_in_machine[q - 1].GetId() + 1, current_candidates[i].GetId() + 1);
            int old_tij = tasks_in_machine[q].GetTime() + Problem::getInstance().CalculateSij(current_candidates[i].GetId() + 1, tasks_in_machine[q].GetId() + 1);
            int aux_tij_1 = tasks_in_machine[q].GetTime() + Problem::getInstance().CalculateSij(tasks_in_machine[q - 1].GetId() + 1, tasks_in_machine[q].GetId() + 1);
            tct_increment += other_tij + (tasks_assigned_size - q + 2) * new_tij + (tasks_assigned_size - q + 1) * (old_tij - aux_tij_1);
          }
        } else if (q == tasks_assigned_size) {
          for (int l = 1; l < tasks_assigned_size; l++) {
            int other_tij = tasks_in_machine[l].GetTime() + Problem::getInstance().CalculateSij(tasks_in_machine[l - 1].GetId() + 1, tasks_in_machine[l].GetId() + 1);
            int new_tij = current_candidates[i].GetTime() + Problem::getInstance().CalculateSij(tasks_in_machine[tasks_assigned_size - 1].GetId() + 1, current_candidates[i].GetId() + 1);
            tct_increment += other_tij + new_tij;
          }
        }
        if (tct_increment < best_tct_increment) {
          best_tct_increment = tct_increment;
          best_task_position = q;
        }
      }
    }
    candidates.push_back(Insertion{current_candidates[best_task_position], best_task_position, best_tct_increment});
    current_candidates.erase(current_candidates.begin() + best_task_position);
  }
  return candidates;
}