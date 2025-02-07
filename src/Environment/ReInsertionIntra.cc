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

/**
 * @brief Generate a new environment
 * @return Solution - The new environment
 */
Solution ReInsertionIntra::GenerateEnvironment() {
  std::vector<Machine> intial_machines = initial_solution_.getMachines();
  std::vector<Machine> new_machines;
  Solution local_optimum = initial_solution_;
  std::vector<std::vector<int>> setup_times = problem_.getSetupTimes();
  for (int i = 0; i < intial_machines.size(); ++i) {
    for (int j = 0; j < intial_machines[i].getTasksAssigned().size(); ++j) {
      for (int k = 0; k < intial_machines[i].getTasksAssigned().size(); ++k) {
        new_machines = intial_machines;
        Task task_to_insert = new_machines[i].getTasksAssigned()[j];
        new_machines[i].RemoveTask(j, setup_times);
        if (j < k && k > 0) {
          new_machines[i].InsertTask(task_to_insert, k - 1, setup_times);
        } else {
          new_machines[i].InsertTask(task_to_insert, k, setup_times);
        }
        Solution new_solution = new_machines;
        if (new_solution.GetTCT() < local_optimum.GetTCT()) { local_optimum = new_solution; }
      }
    }
  }
  local_optimums_.push_back(local_optimum);
  return local_optimum;
}