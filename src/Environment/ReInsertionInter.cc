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
  std::vector<Machine> intial_machines = initial_solution_.getMachines();
  std::vector<Machine> new_machines;
  Solution local_optimum = initial_solution_;
  std::vector<std::vector<int>> setup_times;
  for (int i = 0; i < intial_machines.size(); i++) {
    for (int j = 0; j < intial_machines[i].getTasksAssigned().size(); ++j) {
      for (int k = 0; k < intial_machines.size(); ++k) {
        if (i == k) continue;
        for (int l = 0; l < intial_machines[k].getTasksAssigned().size(); ++l) {
          new_machines = intial_machines;
          setup_times = problem_.getSetupTimes();
          Task task_to_insert = new_machines[i].getTasksAssigned()[j];
          new_machines[i].RemoveTask(j, setup_times);
          new_machines[i].RecalculateTotalCompletionTime(setup_times);
          new_machines[k].InsertTask(task_to_insert, l, setup_times);
          Solution new_solution = new_machines;
          if (new_solution.GetTCT() < local_optimum.GetTCT()) {
            local_optimum = new_solution;
          }
        }
      }
    }
  }
  local_optimums_.push_back(local_optimum);
  return local_optimum;
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
  
  std::vector<Machine> new_machines = intial_machines;
  std::vector<std::vector<int>> setup_times = problem_.getSetupTimes();;
  Task task_to_insert = new_machines[first_machine_index].getTasksAssigned()[first_task_index];
  new_machines[first_machine_index].RemoveTask(first_task_index, setup_times);
  new_machines[first_machine_index].RecalculateTotalCompletionTime(setup_times);
  new_machines[second_machine_index].InsertTask(task_to_insert, second_task_index, setup_times);
  Solution new_solution = new_machines;
  return new_solution;
}