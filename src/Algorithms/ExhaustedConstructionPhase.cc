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
#include <set>

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
  std::vector<Task> tasks_to_assign = problem_.getTasksTimes();
  final_solution_ = problem_.getMachines();
  InitializingMachines(tasks_to_assign);
  // machines_assigned_ = final_solution_.getMachines();
  // std::cout << std::endl;
  std::random_device rd;
  std::mt19937 engine(rd());
  while(tasks_to_assign.size() > 0) {
    // PrintState(tasks_to_assign, machines_assigned_);
    int machine_index_less_span = 0;
    int less_span = 999999;
    for (int i = 0; i < final_solution_.getMachines().size(); i++) {
      int span = final_solution_[i].GetLastTaskTime();
      // std::cout << "Span: " << span << std::endl; 
      if (span < less_span) {
        less_span = span;
        machine_index_less_span = i;
      } 
    }
    // std::cout << "Machine index: " << machine_index_less_span << std::endl;
    std::vector<Insertion> candidates = MakeRandomCandidatesListWithRange(tasks_to_assign, machine_index_less_span);
    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    int random_index = dist(engine);
    Insertion selected_task = candidates[random_index];
    // std::cout << "Candidates size: " << candidates.size() << std::endl;
    // std::cout << "Selected task: " << selected_task.task.GetId() << std::endl;
    // std::cout << "Selected task position: " << selected_task.dest_task_index << std::endl;
    // std::cout << "Selected task increment TCT: " << selected_task.increment_tct << std::endl;
    // machines_assigned_[machine_index_less_span].InsertTask(selected_task.task, selected_task.dest_task_index, selected_task.increment_tct);
    final_solution_.InsertTask(selected_task.task, machine_index_less_span, selected_task.dest_task_index, selected_task.increment_tct);
    tasks_to_assign.erase(std::remove(tasks_to_assign.begin(), tasks_to_assign.end(), selected_task.task));
  }
  return final_solution_;
}

/**
 * @brief Inicialización de las máquinas con una tarea
 * @param tasks_to_assign - Tareas a asignar
 */
void ExhaustedConstructionPhase::InitializingMachines(std::vector<Task>& tasks_to_assign) { // Hay que modificar y añadir S0j
  for (int i = 0; i < final_solution_.getMachines().size(); i++) {
    int best_time = 999999;
    int best_task_index = 0;
    for (int j = 0; j < tasks_to_assign.size(); j++) {
      int t0j = tasks_to_assign[j].GetTime() + problem_.CalculateSij(0, tasks_to_assign[j].GetId() + 1); 
      if (t0j < best_time) {
        best_time = t0j;
        best_task_index = j;
      }
    }
    final_solution_.InsertTask(tasks_to_assign[best_task_index], i, 0, best_time);
    tasks_to_assign.erase(tasks_to_assign.begin() + best_task_index);
  }
}

/**
 * @brief Creación de una lista de candidatos aleatorizada
 * @param tasks_to_assign - Tareas a asignar
 * @return Lista de candidatos
 */
std::vector<Insertion> ExhaustedConstructionPhase::MakeRandomCandidatesList(const std::vector<Task>& tasks_candidates, int chosen_machine_index) {
  std::vector<Task> current_candidates = tasks_candidates;
  std::vector<Insertion> candidates;
  int tasks_assigned = final_solution_[chosen_machine_index].getTasksAssigned().size();
  for (int k = 0; k < RCL_size_; k++) {
    if (current_candidates.empty()) { break; }
    int best_tct_increment = 9999999;
    int best_position_to_insert = -1;
    int task_selected = -1;
    for (int i = 0; i < current_candidates.size(); i++) {
      for (int q = 0; q <= tasks_assigned; q++) {       // q Es la posición de la tarea en la máquina
        int tct_increment = final_solution_[chosen_machine_index].EmulateInsertion(current_candidates[i], q);
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

/**
 * @brief Creación de una lista de candidatos aleatorizada usando un 
 *        alpha que marca el rango de candidatos
 * @param tasks_candidates - Tareas candidatas
 * @param chosen_machine_index - Índice de la máquina elegida
 * @return Lista de candidatos
 */
std::vector<Insertion> ExhaustedConstructionPhase::MakeRandomCandidatesListWithRange(const std::vector<Task>& tasks_candidates, int chosen_machine_index) {
  std::vector<Insertion> candidates;
  std::vector<Insertion> RCL;
  int best_tct_increment = 9999999;
  int worst_tct_increment = -1;
  int tasks_assigned = final_solution_[chosen_machine_index].getTasksAssigned().size();
  for (int i = 0; i < tasks_candidates.size(); i++) {
    int best_i_tct = 9999999;
    int best_qi = -1;
    for (int q = 0; q <= tasks_assigned; q++) {       // q Es la posición de la tarea en la máquina
      int tct_increment = final_solution_[chosen_machine_index].EmulateInsertion(tasks_candidates[i], q);
      best_tct_increment = best_tct_increment < tct_increment ? best_tct_increment : tct_increment;
      worst_tct_increment = worst_tct_increment > tct_increment ? worst_tct_increment : tct_increment;
      if (tct_increment < best_i_tct) {
        best_i_tct = tct_increment;
        best_qi = q;
      }
    }
    RCL.push_back(Insertion{tasks_candidates[i], best_qi, best_i_tct});
  }
  int range = best_tct_increment + alpha_ * (worst_tct_increment - best_tct_increment);
  for (const auto& insertion : RCL) {
    if (insertion.increment_tct < range) { 
      candidates.push_back(insertion);
    }
  }
  return candidates;
}