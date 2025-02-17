/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief SwapIntra methods declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Environment/SwapIntra.h"

Solution SwapIntra::GenerateEnvironment() {
  bool mejora = true;
  Solution final_solution = initial_solution_;
  best_solution_ = initial_solution_;
  while (mejora) {
    Movement best_movement = EmulateMovements(final_solution);
    std::cout << "Best movement: " << best_movement.orig_machine_index << " " << best_movement.orig_task_index << " " << best_movement.dest_machine_index << " " << best_movement.dest_task_index << " " << best_movement.tct_increment << std::endl;
    ApplyMovement(final_solution, best_movement);
    std::cout << "Final solution: \n" << final_solution << std::endl;
    int diference = final_solution.GetTCT();
    final_solution.RecalculateTotalCompletionTime();
    diference -= final_solution.GetTCT();
    std::cout << "Diference: " << diference << std::endl;
    if (best_solution_.GetTCT() <= final_solution.GetTCT()) {
      final_solution = best_solution_;
      mejora = false;
    } else {
      best_solution_ = final_solution;
    }
  }
  return final_solution;
}

Movement SwapIntra::EmulateMovements(const Solution& solution) {
  int best_tct_movement = 9999999;
  Movement best_movement{-1, -1, -1, -1, -1};
  for (int m = 0; m < solution.getMachines().size(); m++) {
    int tasks_assigned_size = solution[m].getTasksAssigned().size();
    for (int i = 0; i < tasks_assigned_size; i++) {
      for (int j = i + 2; j < tasks_assigned_size; j++) {
        // std::cout << "Machine: " << m << " Task i: " << i << " Task j: " << j << std::endl;
        int delta_tct = solution.EmulateSwapIntra(m, i, j);
        // std::cout << "Delta TCT: " << delta_tct << std::endl;
        if (delta_tct < best_tct_movement) {
          best_tct_movement = delta_tct;
          best_movement = {m, i, m, j, delta_tct};
        }
      }
    }
  }
  return best_movement;
}

void SwapIntra::ApplyMovement(Solution& solution, const Movement& movement) {
  solution.ExchangeTasks(movement.orig_machine_index, movement.orig_task_index, movement.dest_task_index);
  solution.AddTCT(movement.tct_increment);
}