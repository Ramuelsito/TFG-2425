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

void PrintState(const std::vector<Task>& tasks_to_assign, const std::vector<Machine>& machines_assigned) {
  std::cout << "Tasks to assign: ";
  for (int i = 0; i < tasks_to_assign.size(); i++) {
    std::cout << tasks_to_assign[i].GetId() << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < machines_assigned.size(); i++) {
    std::cout << "Machine " << i << ": ";
    std::vector<Task> tasks_assigned = machines_assigned[i].getTasksAssigned();
    for (int j = 0; j < tasks_assigned.size(); j++) {
      std::cout << tasks_assigned[j].GetId() << " ";
    }
    std::cout << std::endl;
  }
}

/**
 * @brief Construcción de una solución inicial mediante un algoritmo greedy con cierta aleatoriedad
 * @return Solución obtenida
 */
Solution ExhaustedConstructionPhase::ConstructGreedyRandSolution() {
  std::vector<Task> tasks_to_assign = problem_->getTasksTimes();
  machines_assigned_ = problem_->getMachines();
  std::vector<std::vector<int>> setup_times = problem_->getSetupTimes();
  InitializingMachines(tasks_to_assign);
  std::cout << std::endl;
  std::random_device rd;
  std::mt19937 engine(rd());
  while(tasks_to_assign.size() > 0) {
    PrintState(tasks_to_assign, machines_assigned_);
    int machine_index_less_span = 0;
    int less_span = 999999;
    for (int i = 0; i < machines_assigned_.size(); i++) {
      int span = machines_assigned_[i].GetLastTaskTime();
      std::cout << "Span: " << span << std::endl; 
      if (span < less_span) {
        less_span = span;
        machine_index_less_span = i;
      } 
    }
    std::cout << "Machine index: " << machine_index_less_span << std::endl;
    std::vector<Insertion> candidates = MakeRandomCandidatesList(tasks_to_assign, machine_index_less_span);
    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    int random_index = dist(engine);
    Insertion selected_task = candidates[random_index];
    std::cout << "Selected task: " << selected_task.task.GetId() << std::endl;
    std::cout << "Selected task position: " << selected_task.dest_task_index << std::endl;
    std::cout << "Selected task increment TCT: " << selected_task.increment_tct << std::endl;
    machines_assigned_[machine_index_less_span].InsertTask(selected_task.task, selected_task.dest_task_index, selected_task.increment_tct);
    tasks_assigned_.push_back(selected_task.task.GetId());
    tasks_to_assign.erase(std::remove(tasks_to_assign.begin(), tasks_to_assign.end(), selected_task.task));
  }
  Solution solution(machines_assigned_);
  return solution;
}

/**
 * @brief Inicialización de las máquinas con una tarea
 * @param tasks_to_assign - Tareas a asignar
 */
void ExhaustedConstructionPhase::InitializingMachines(std::vector<Task>& tasks_to_assign) { // Hay que modificar y añadir S0j
  for (int i = 0; i < machines_assigned_.size(); i++) {
    int best_time = 999999;
    int best_task_index = 0;
    for (int j = 0; j < tasks_to_assign.size(); j++) {
      int t0j = tasks_to_assign[j].GetTime() + problem_->CalculateSij(0, tasks_to_assign[j].GetId() + 1); 
      if (t0j < best_time) {
        best_time = t0j;
        best_task_index = j;
      }
    }
    machines_assigned_[i].InsertTask(tasks_to_assign[best_task_index], 0, best_time);
    tasks_assigned_.push_back(tasks_to_assign[best_task_index].GetId());
    tasks_to_assign.erase(tasks_to_assign.begin() + best_task_index);
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
  for (int k = 0; k < RCL_size_; k++) {
    if (current_candidates.empty()) { break; }
    int best_tct_increment = 9999999;
    int best_position_to_insert = -1;
    int task_selected = -1;
    int tasks_assigned_size = tasks_in_machine.size();
    for (int i = 0; i < current_candidates.size(); i++) {
      for (int q = 0; q <= tasks_assigned_size; q++) {       // q Es la posición de la tarea en la máquina
        int tct_increment = 0;
        if (q == 0) {
          int new_t0i = current_candidates[i].GetTime() + problem_->CalculateSij(0, current_candidates[i].GetId() + 1);
          int old_t01 = tasks_in_machine[0].GetTime() + problem_->CalculateSij(0, tasks_in_machine[0].GetId() + 1);
          int new_tij = tasks_in_machine[0].GetTime() + problem_->CalculateSij(current_candidates[i].GetId() + 1, tasks_in_machine[0].GetId() + 1);
          tct_increment = (tasks_assigned_size + 1) * new_t0i + tasks_assigned_size * (new_tij - old_t01);
        } else if((1 <= q) && (q < tasks_assigned_size)) {
          int other_t01 = tasks_in_machine[0].GetTime() + problem_->CalculateSij(0, tasks_in_machine[0].GetId() + 1);
          tct_increment = other_t01;
          for (int l = 1; l <= q - 1; l++) {
            int other_tij = tasks_in_machine[l].GetTime() + problem_->CalculateSij(tasks_in_machine[l - 1].GetId() + 1, tasks_in_machine[l].GetId() + 1);
            tct_increment += other_tij;
          }
          int new_tij = current_candidates[i].GetTime() + problem_->CalculateSij(tasks_in_machine[q - 1].GetId() + 1, current_candidates[i].GetId() + 1);
          int new_tiq = tasks_in_machine[q].GetTime() + problem_->CalculateSij(current_candidates[i].GetId() + 1, tasks_in_machine[q].GetId() + 1);
          int aux_tij = tasks_in_machine[q].GetTime() + problem_->CalculateSij(tasks_in_machine[q - 1].GetId() + 1, tasks_in_machine[q].GetId() + 1);
          tct_increment += (tasks_assigned_size - q + 2 + 1) * new_tij + (tasks_assigned_size - q + 1 + 1) * (new_tiq - aux_tij);
        } else if (q == tasks_assigned_size) { // Tenemos que tener cuidado porque si q = tasks_assigned_size, tasks_in_machine[q] no existe
          int other_t01 = tasks_in_machine[0].GetTime() + problem_->CalculateSij(0, tasks_in_machine[0].GetId() + 1);
          tct_increment = other_t01;
          for (int l = 1; l < tasks_assigned_size; l++) {
            int other_tij = tasks_in_machine[l].GetTime() + problem_->CalculateSij(tasks_in_machine[l - 1].GetId() + 1, tasks_in_machine[l].GetId() + 1);
            tct_increment += other_tij;
          }
          int new_tij = current_candidates[i].GetTime() + problem_->CalculateSij(tasks_in_machine[tasks_assigned_size - 1].GetId() + 1, current_candidates[i].GetId() + 1);
          tct_increment += new_tij;
        }
        if (tct_increment < best_tct_increment) {
          best_tct_increment = tct_increment;
          best_position_to_insert = q;
          task_selected = i;
        }
      }
    }
    candidates.push_back(Insertion{current_candidates[task_selected], best_position_to_insert, best_tct_increment});
    current_candidates.erase(current_candidates.begin() + task_selected);
  }
  return candidates;
}