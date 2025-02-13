/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief ReInsertionInter methods declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Environment/ReInsertionInter.h"
#include <random>

Solution ReInsertionInter::GenerateEnvironment() {
  std::vector<Machine> new_machines;
  Solution final_solution = initial_solution_;
  
  return final_solution;
}

/**
 * @brief Selects a random neighbor from the current solution
 * @return The new solution
 */
Solution ReInsertionInter::SelectRandomNeighbor() {
  std::vector<Machine> intial_machines = initial_solution_.getMachines();

  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<> dist(0, intial_machines.size() - 1);
  int first_machine_index = dist(engine);                                           //  Obtain a random machine index
  int second_machine_index = dist(engine);                                          //  Obtain another random machine index
  if (first_machine_index == second_machine_index) {
    second_machine_index = (second_machine_index + 1) % intial_machines.size();
  }
  std::uniform_int_distribution<> dist2(0, intial_machines[first_machine_index].getTasksAssigned().size() - 1);
  int first_task_index = dist2(engine);                                            // Obtain a random task index
  std::uniform_int_distribution<> dist3(0, intial_machines[second_machine_index].getTasksAssigned().size() - 1);
  int second_task_index = dist3(engine);                                           // Obtain another random task index
  
  if (first_task_index == 0) {
    return initial_solution_;
  }

  Movement movement = {first_machine_index, second_machine_index, first_task_index, second_task_index};
  Solution new_solution = ApplyMovement(initial_solution_, movement);  

  // std::vector<Machine> new_machines = intial_machines;
  // std::vector<std::vector<int>> setup_times = problem_.getSetupTimes();
  // Task task_to_insert = new_machines[first_machine_index].getTasksAssigned()[first_task_index];
  // new_machines[first_machine_index].RemoveTask(first_task_index, setup_times);
  // new_machines[first_machine_index].RecalculateTotalCompletionTime(setup_times);
  // new_machines[second_machine_index].InsertTask(task_to_insert, second_task_index, setup_times);
  // Solution new_solution = new_machines;
  return new_solution;
}

/**
 * @brief Emulates the movement calculating only the TCT
 * @param solution The current solution
 * @return The best movement found
 */
Movement ReInsertionInter::EmulateMovements(const Solution& solution) {
  std::vector<Machine> initial_machines = initial_solution_.getMachines();
  int best_tct_movement = 0;
  Movement best_movement;
  for (int m1 = 0; m1 < initial_machines.size(); m1++) {
    int tasks_assigned_machine1 = initial_machines[m1].getTasksAssigned().size();
    for (int i = 0; i < initial_machines[m1].getTasksAssigned().size(); ++i) {
      int tct_extraction = initial_machines[m1].EmulateRemoval(i);
      for (int m2 = 0; m2 < initial_machines.size(); ++m2) {
        int tasks_assigned_machine2 = initial_machines[m2].getTasksAssigned().size();
        if (m1 == m2) continue;
        for (int j = 0; j < initial_machines[m2].getTasksAssigned().size(); ++j) {
          // Dentro de estos for, tengo que calcular el TCT de la nueva solución
          // y quedarme con la mejor
          int tct_movement = 0;
          int tct_insertion = 0;
          if (tct_movement < best_tct_movement) {
            best_tct_movement = tct_movement;
            best_movement = {m1, m2, i, j, tct_movement, 0};
          }
        }
      }
    }
  }
  return best_movement;
}

/**
 * @brief Apply the movement to the solution
 * @param solution The current solution
 * @param movement The movement to apply
 * @return The new solution
 */
Solution ReInsertionInter::ApplyMovement(const Solution& solution, const Movement& movement) {
  std::vector<Machine> intial_machines = solution.getMachines();
  std::vector<Machine> new_machines = intial_machines;
  // new_machines[movement.orig_machine_index].RemoveTask(movement.orig_task_index);
  // int new_tct = new_machines[movement.dest_machine_index].GetTotalTime() - 0;
  // new_machines[movement.orig_machine_index].SetTotalTime(new_tct);
  // new_machines[movement.dest_machine_index].InsertTask(intial_machines[movement.dest_machine_index].getTasksAssigned()[movement.orig_task_index], movement.dest_task_index);
  Solution new_solution = new_machines;
  return new_solution;
}