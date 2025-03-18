/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Main file for the Parallel Machine Scheduling problem
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../Includes/Problem.h"
#include "../Includes/Solution.h"
#include "../Includes/Algorithms/MultiGVNS.h"
#include "../Includes/Util.h"
#include "../Includes/FLA/InstanceData.h"
#include <chrono>
#include <filesystem>

int main(int argc, char* argv[]) {
  const std::string path = "../Instances/";
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error("Directory [" + path + "] does not exist.");
  }
  if (argc <= 1) { return -1; }
  if (std::string(argv[1]) == "-h") {
    Usage();
    return 0;
  }
  std::unordered_map<Solution, Solution, int> data_bank;
  if (std::string(argv[1]) == "-all") {
    Solution solution;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      Problem::getInstance(entry.path().string());
      MultiGVNS multigvns(40);
      solution = multigvns.Solve(data_bank);
      std::cout << solution << std::endl;
    }
    return 0;
  } else {
    // int algorithmOption = AlgorithmMenu();
    std::string instance = argv[1];
    std::cout << "../Instances/" + instance + ".txt" << std::endl;
    Problem& problem = Problem::getInstance("../Instances/" + instance + ".txt");
    std::cout << problem << std::endl;
    InstanceData data;
    std::cout << data << std::endl;
    // Solution solution;
    // std::string algorithm_name;
    // std::chrono::seconds performance_time;
    // double update_percentage;
    // algorithm_name = "GVNS";
    // MultiGVNS multigvns(40);
    // auto start = std::chrono::steady_clock::now();
    // solution = multigvns.Solve(data_bank);
    // auto end = std::chrono::steady_clock::now();
    // update_percentage = multigvns.GetUpdatePercentage();
    // performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    // solution.PrintStudiedSolution(instance, algorithm_name, performance_time.count(), Problem::getInstance().getTasksTimes().size());
    // std::cout << solution << std::endl << "Performance time: " << performance_time.count() << " seconds" << std::endl << "Update percentage: " << update_percentage << "%" << std::endl;
    // for (int i = 0; i < initial_solution.getMachines()[0].getTasksAssigned().size(); i++) {
    //   std::vector<Machine> machines = initial_solution.getMachines();
    //   std::cout << initial_solution << std::endl;
    //   std::cout << "Task to remove: " << machines[0].getTasksAssigned()[i] << std::endl;
    //   int tct_decrement = machines[0].EmulateRemoval(i);
    //   machines[0].RemoveTask(i, tct_decrement);
    //   Solution new_solution(machines);
    //   int diference = new_solution.GetTCT();
    //   std::cout << new_solution << std::endl;
    //   new_solution.RecalculateTotalCompletionTime();
    //   diference = diference - new_solution.GetTCT();
    //   std::cout << new_solution << std::endl;
    //   std::cout << "Diference: " << diference << std::endl;
    // }
  }
  return 0;
}
