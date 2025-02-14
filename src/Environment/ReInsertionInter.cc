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
  // Saturar
  std::vector<Machine> new_machines;
  Solution final_solution = initial_solution_;
  Movement best_movement = EmulateMovements(final_solution);
  std::cout << "Best movement: " << best_movement.orig_machine_index << " " << best_movement.orig_task_index << " " << best_movement.dest_machine_index << " " << best_movement.dest_task_index << " " << best_movement.tct_increment << " " << best_movement.tct_decrement << std::endl;
  final_solution = ApplyMovement(final_solution, best_movement);
  return final_solution;
}

/**
 * @brief Selects a random neighbor from the current solution
 * @return The new solution
 */
Solution ReInsertionInter::SelectRandomNeighbor() {
  std::vector<Machine> initial_machines = initial_solution_.getMachines();

  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<> dist(0, initial_machines.size() - 1);
  int first_machine_index = dist(engine);                                           //  Obtain a random machine index
  int second_machine_index = dist(engine);                                          //  Obtain another random machine index
  if (first_machine_index == second_machine_index) {
    second_machine_index = (second_machine_index + 1) % initial_machines.size();
  }
  std::uniform_int_distribution<> dist2(0, initial_machines[first_machine_index].getTasksAssigned().size() - 1);
  int first_task_index = dist2(engine);                                            // Obtain a random task index
  std::uniform_int_distribution<> dist3(0, initial_machines[second_machine_index].getTasksAssigned().size() - 1);
  int second_task_index = dist3(engine);                                           // Obtain another random task index
  int tct_decrement = initial_machines[first_machine_index].EmulateRemoval(first_task_index);
  Task task_to_insert = initial_machines[first_machine_index].getTasksAssigned()[first_task_index];
  int tct_increment = initial_machines[second_machine_index].EmulateInsertion(task_to_insert, second_task_index);
  Movement movement = {first_machine_index, first_task_index, second_machine_index, second_task_index, tct_increment, tct_decrement};
  Solution new_solution = ApplyMovement(initial_solution_, movement);  
  return new_solution;
}

/**
 * @brief Emulates the movement calculating only the TCT
 * @param solution The current solution
 * @return The best movement found
 */
Movement ReInsertionInter::EmulateMovements(const Solution& solution) {
  std::vector<Machine> initial_machines = initial_solution_.getMachines();
  int best_tct_movement = 9999999;
  Movement best_movement{-1, -1, -1, -1, -1, -1};
  for (int m1 = 0; m1 < initial_machines.size(); m1++) {
    for (int i = 0; i < initial_machines[m1].getTasksAssigned().size(); ++i) {
      int tct_extraction = initial_machines[m1].EmulateRemoval(i);
      for (int m2 = 0; m2 < initial_machines.size(); ++m2) {
        if (m1 == m2) continue;
        for (int j = 0; j <= initial_machines[m2].getTasksAssigned().size(); ++j) {
          // Dentro de estos for, tengo que calcular el TCT de la nueva solución
          // y quedarme con la mejor
          int tct_insertion = initial_machines[m2].EmulateInsertion(initial_machines[m1].getTasksAssigned()[i], j);
          int tct_movement = tct_extraction + tct_insertion;
          if (tct_movement < best_tct_movement) {
            best_tct_movement = tct_movement;
            best_movement = {m1, i, m2, j, tct_insertion, tct_extraction};
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
  std::vector<Machine> new_solution = solution.getMachines();
  Task task_to_insert = new_solution[movement.orig_machine_index].getTasksAssigned()[movement.orig_task_index];
  new_solution[movement.orig_machine_index].RemoveTask(movement.orig_task_index, movement.tct_decrement);
  new_solution[movement.dest_machine_index].InsertTask(task_to_insert, movement.dest_task_index, movement.tct_increment);
  return new_solution;
}