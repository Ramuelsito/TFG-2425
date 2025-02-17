/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief SwapInter methods declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Environment/SwapInter.h"

Solution SwapInter::GenerateEnvironment() {
  bool mejora = true;
  Solution final_solution = initial_solution_;
  best_solution_ = initial_solution_;
  while (mejora) {
    Movement best_movement = EmulateMovements(final_solution);
    // std::cout << "Best movement: " << best_movement.orig_machine_index << " " << best_movement.orig_task_index << " " << best_movement.dest_machine_index << " " << best_movement.dest_task_index << " " << best_movement.tct_increment << std::endl;
    ApplyMovement(final_solution, best_movement);
    // std::cout << "Final solution: \n" << final_solution << std::endl;
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

Movement SwapInter::EmulateMovements(const Solution& solution) {
  int best_tct_movement = 9999999;
  Movement best_movement{-1, -1, -1, -1, -1};
  int machine_size = solution.getMachines().size();
  for (int m1 = 0; m1 < machine_size; m1++) {
    int first_machine_size = solution[m1].getTasksAssigned().size();
    for (int i = 0; i < first_machine_size; i++) {
      for (int m2 = 0; m2 < machine_size; m2++) {
        int second_machine_size = solution[m2].getTasksAssigned().size();
        if (m1 == m2) { continue; }
        for (int j = 0; j < second_machine_size; j++) {
          int delta_tct = solution.EmulateSwapInter(m1, i, m2, j);
          if (delta_tct < best_tct_movement) {
            best_tct_movement = delta_tct;
            best_movement = {m1, i, m2, j, delta_tct};
          }
        }
      }
    }
  }
  return best_movement;
}

void SwapInter::ApplyMovement(Solution& solution, const Movement& movement) {
  // Se tienen que utilizar los métodos ya implementados, de insertar y eliminar tareas
  Task first_task = solution[movement.orig_machine_index][movement.orig_task_index];
  Task second_task = solution[movement.dest_machine_index][movement.dest_task_index];
  solution.RemoveTask(movement.orig_machine_index, movement.orig_task_index);
  solution.RemoveTask(movement.dest_machine_index, movement.dest_task_index);
  solution.InsertTask(first_task, movement.dest_machine_index, movement.dest_task_index);
  solution.InsertTask(second_task, movement.orig_machine_index, movement.orig_task_index); 
  solution.AddTCT(movement.tct_increment);
}