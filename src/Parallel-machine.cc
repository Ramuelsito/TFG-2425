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
#include "../Includes/Instances/InstanceData.h"
#include "../Includes/Instances/Instance.h"
#include "../Includes/FLA/StudiedSolution.h"
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
  if (std::string(argv[1]) == "-all") {
    Solution solution;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      Problem::getInstance(entry.path().string());
      MultiGVNS multigvns(40);
      solution = multigvns.Solve();
      std::cout << solution << std::endl;
    }
    return 0;
  } else if (std::string(argv[1]) == "-gen") {
    int number_of_tasks = std::stoi(argv[2]);
    int number_of_machines = std::stoi(argv[3]);
    std::string tasks_distribution = argv[4];
    std::string setup_distribution = argv[5];
    Instance instance(number_of_tasks, number_of_machines, tasks_distribution, setup_distribution);
    instance.GenerateInstance(4, 0.7);
    instance.SaveInstance(); 
  } else {
    // int algorithmOption = AlgorithmMenu();
    std::string instance = argv[1];
    Problem& problem = Problem::getInstance("../Instances/" + instance + ".txt");
    std::cout << problem << std::endl;
    // InstanceData data;
    // std::cout << data << std::endl;
    Solution solution;
    std::string algorithm_name;
    std::chrono::seconds performance_time;
    double update_percentage;
    algorithm_name = "GVNS";
    MultiGVNS multigvns(40);
    auto start = std::chrono::steady_clock::now();
    solution = multigvns.Solve();
    auto end = std::chrono::steady_clock::now();
    update_percentage = multigvns.GetUpdatePercentage();
    performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    solution.PrintStudiedSolution(instance, algorithm_name, performance_time.count(), Problem::getInstance().getTasksTimes().size());
    std::cout << solution << std::endl << "Performance time: " << performance_time.count() << " seconds" << std::endl << "Update percentage: " << update_percentage << "%" << std::endl;
    std::cout << "Neighborhood data: " << std::endl;
    std::cout << multigvns.GetNeighborhoodData() << std::endl;
  }
  return 0;
}
