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

  std::vector<int> non_empty_machines;            // Vector to store indices of machines with at least one task
  for (int i = 0; i < number_of_machines; ++i) {
    if (!new_solution[i].getTasksAssigned().empty()) {
      non_empty_machines.push_back(i);
    }
  }
  if (non_empty_machines.empty()) {
    return new_solution;                          // No hay máquinas con tareas, no se puede hacer movimiento
  }

  // 2. Elegir aleatoriamente la máquina origen (no vacía)
  std::uniform_int_distribution<> dist_non_empty(0, non_empty_machines.size() - 1);
  int first_machine_index = non_empty_machines[dist_non_empty(engine)];

  // 3. Elegir aleatoriamente la máquina destino (distinta de la origen)
  std::uniform_int_distribution<> dist_dest(0, number_of_machines - 1);
  int idx = dist_dest(engine);
  // Saltar la máquina origen
  int second_machine_index = (idx == first_machine_index) ? (idx + 1) % number_of_machines : idx;

  int num_tasks_first = new_solution[first_machine_index].getTasksAssigned().size();
  std::uniform_int_distribution<> dist_task(0, num_tasks_first - 1);
  int first_task_index = dist_task(engine);

  int num_tasks_second = new_solution[second_machine_index].getTasksAssigned().size() == 0 ? 0 : new_solution[second_machine_index].getTasksAssigned().size() - 1; // If the second machine has no tasks, set to 0
  std::uniform_int_distribution<> dist_pos(0, num_tasks_second);
  int second_task_index = dist_pos(engine);                                      // Obtain another random task index

  // std::cout << "First task index: " << first_task_index << " Second task index: " << second_task_index << std::endl;
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
  int best_tct_movement = 9999999;
  Movement best_movement{-1, -1, -1, -1, -1};
  int number_machines = solution.getMachines().size();
  for (int m1 = 0; m1 < number_machines; m1++) {
    for (int i = 0; i < solution[m1].size(); ++i) {
      int tct_extraction = solution[m1].EmulateRemoval(i);
      for (int m2 = 0; m2 < number_machines; ++m2) {
        if (m1 == m2) continue;
        for (int j = 0; j <= solution[m2].size(); ++j) {
          int tct_insertion = solution[m2].EmulateInsertion(solution[m1][i], j);
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