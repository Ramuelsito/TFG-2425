/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Intra methods implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Environment/ReInsertionIntra.h"

Solution ReInsertionIntra::GenerateEnvironment() {
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

/**
 * @brief Emulates the movement calculating only the TCT
 * @param solution The current solution
 * @return The best movement found
 */
Movement ReInsertionIntra::EmulateMovements(const Solution& solution) {
  int best_tct_movement = 9999999;
  Movement best_movement{-1, -1, -1, -1, -1};
  int machines = solution.getMachines().size();
  for (int m = 0; m < machines; m++) {
    for (int i = 0; i < solution[m].size() - 1; ++i) {
      int delta_tct = solution.EmulateReInsertion(m, i);
      if (delta_tct < best_tct_movement) {
        best_tct_movement = delta_tct;
        best_movement = {m, i, m, i + 1, delta_tct};
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
void ReInsertionIntra::ApplyMovement(Solution& solution, const Movement& movement) {
  solution.ExchangeTasks(movement.orig_machine_index, movement.orig_task_index, movement.dest_task_index);
  solution.AddTCT(movement.tct_increment);
}