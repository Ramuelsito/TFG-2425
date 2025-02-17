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
  bool mejora = true;
  Solution final_solution = initial_solution_;
  best_solution_ = initial_solution_;
  while (mejora) {
    Movement best_movement = EmulateMovements(final_solution);
    // std::cout << "Best movement: " << best_movement.orig_machine_index << " " << best_movement.orig_task_index << " " << best_movement.dest_machine_index << " " << best_movement.dest_task_index << " " << best_movement.tct_increment << " " << best_movement.tct_decrement << std::endl;
    ApplyMovement(final_solution, best_movement);
    // std::cout << "Final solution: " << final_solution << std::endl;
    // int diference = final_solution.GetTCT();
    // final_solution.RecalculateTotalCompletionTime();
    // diference -= final_solution.GetTCT();
    // std::cout << "Diference: " << diference << std::endl;
    if (best_solution_.GetTCT() <= final_solution.GetTCT()) {
      final_solution = best_solution_;
      mejora = false;
    } else {
      best_solution_ = final_solution;
    }
  }
  return final_solution;
}

/**
 * @brief Selects a random neighbor from the current solution
 * @return The new solution
 */
Solution ReInsertionInter::SelectRandomNeighbor() {
  Solution new_solution = initial_solution_;
  std::random_device rd;
  std::mt19937 engine(rd());

  int number_of_machines = new_solution.getMachines().size();
  std::uniform_int_distribution<> dist(0, number_of_machines - 1);
  int first_machine_index = dist(engine);                                           //  Obtain a random machine index
  int second_machine_index = dist(engine);                                          //  Obtain another random machine index
  if (first_machine_index == second_machine_index) {
    second_machine_index = (second_machine_index + 1) % number_of_machines;
  }
  std::uniform_int_distribution<> dist2(0, new_solution[first_machine_index].getTasksAssigned().size() - 1);
  int first_task_index = dist2(engine);                                            // Obtain a random task index
  std::uniform_int_distribution<> dist3(0, new_solution[second_machine_index].getTasksAssigned().size() - 1);
  int second_task_index = dist3(engine);                                           // Obtain another random task index
  int tct_decrement = new_solution[first_machine_index].EmulateRemoval(first_task_index);
  Task task_to_insert = new_solution[first_machine_index][first_task_index];
  int tct_increment = new_solution[second_machine_index].EmulateInsertion(task_to_insert, second_task_index);
  Movement movement = {first_machine_index, first_task_index, second_machine_index, second_task_index, (tct_increment + tct_decrement)};
  ApplyMovement(new_solution, movement);  
  return new_solution;
}

/**
 * @brief Emulates the movement calculating only the TCT
 * @param solution The current solution
 * @return The best movement found
 */
Movement ReInsertionInter::EmulateMovements(const Solution& solution) {
  std::vector<Machine> initial_machines = solution.getMachines();
  int best_tct_movement = 9999999;
  Movement best_movement{-1, -1, -1, -1, -1};
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
            best_movement = {m1, i, m2, j, tct_movement};
          }
        }
      }
    }
  }
  return best_movement;
}

/**
 * @brief Apply the movement to the solution
 * @param solution The solution where the movement is going to be applied
 * @param movement The movement to apply
 * @return void
 */
void ReInsertionInter::ApplyMovement(Solution& solution, const Movement& movement) {
  Task task_to_insert = solution[movement.orig_machine_index][movement.orig_task_index];
  solution.RemoveTask(movement.orig_machine_index, movement.orig_task_index);
  solution.InsertTask(task_to_insert, movement.dest_machine_index, movement.dest_task_index);
  solution.AddTCT(movement.tct_increment);
}