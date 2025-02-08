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
  std::vector<Machine> intial_machines = initial_solution_.getMachines();
  std::vector<Machine> new_machines;
  Solution local_optimum = initial_solution_;
  // std::vector<std::vector<int>> setup_times;
  // for (int i = 0; i < intial_machines.size(); i++) {
  //   for (int j = 0; j < intial_machines[i].getTasksAssigned().size() - 1; ++j) {
  //     for (int k = j + 1; k < intial_machines[i].getTasksAssigned().size(); ++k) {
  //       new_machines = intial_machines;
  //       setup_times = problem_.getSetupTimes();
  //       // new_machines[i].SwapTasks(j, k, setup_times);
  //       Task task_to_insert = new_machines[i].getTasksAssigned()[j];
  //       Task second_task_to_insert = new_machines[i].getTasksAssigned()[k];
  //       new_machines[i].RemoveTask(j, setup_times);
  //       if (j < k && k > 0) {
  //         new_machines[i].RemoveTask(k - 1, setup_times);
  //         new_machines[i].InsertTask(task_to_insert, k - 1, setup_times);
  //       } else {
  //         new_machines[i].RemoveTask(k, setup_times);
  //         new_machines[i].InsertTask(task_to_insert, k, setup_times);
  //       }
  //       new_machines[i].InsertTask(second_task_to_insert, j, setup_times);
  //       Solution new_solution = new_machines;
  //       if (new_solution.GetTCT() < local_optimum.GetTCT()) {
  //         local_optimum = new_solution;
  //       }
  //     }
  //   }
  // }
  // local_optimums_.push_back(local_optimum);
  return local_optimum;
}