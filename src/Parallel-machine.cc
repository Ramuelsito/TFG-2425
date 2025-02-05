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
#include "../Includes/Algorithms/Greedy.h"
#include "../Includes/Algorithms/GRASP-Algorithms/GRASP-min.h"
#include "../Includes/Algorithms/MultiGVNS.h"
#include "../Includes/Util.h"
#include <chrono>
#include <filesystem>

int main(int argc, char* argv[]) {
  try {
    const std::string path = "../Instances/";
    if (!std::filesystem::exists(path)) {
      throw std::runtime_error("Directory [" + path + "] does not exist.");
    }
    if (argc > 1) {
      if (std::string(argv[1]) == "-h") {
        Usage();
        return 0;
      }
      if (std::string(argv[1]) == "-all") {
        Solution solution;
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
          Problem problem(entry.path().string());
          MultiGVNS multigvns(problem);
          solution = multigvns.Solve();
          std::cout << solution << std::endl;
        }
        return 0;
      } else {
        int algorithmOption = AlgorithmMenu();
        std::string instance = argv[1];
        std::cout << "../Instances/" + instance + ".txt" << std::endl;
        Problem problem("../Instances/" + instance + ".txt");

        std::cout << "Problem: " << std::endl;
        std::cout << problem << std::endl;

        Solution solution;
        std::string algorithm_name;
        std::chrono::seconds performance_time;
        double update_percentage;
      //   switch (algorithmOption) {
      //   case 1: {
      //     algorithm_name = "Greedy";
      //     Greedy greedy(problem);
      //     auto start = std::chrono::steady_clock::now();
      //     solution = greedy.Solve();
      //     auto end = std::chrono::steady_clock::now();
      //     performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
      //     break;
      //   }
      //   case 2: {
      //     algorithm_name = "Grasp";
      //     GRASPMin grasp_min(problem);
      //     auto start = std::chrono::steady_clock::now();
      //     solution = grasp_min.Solve();
      //     auto end = std::chrono::steady_clock::now();
      //     performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
      //     break;
      //   }
      //   case 3: {
      //     algorithm_name = "GVNS";
      //     MultiGVNS multigvns(problem);
      //     auto start = std::chrono::steady_clock::now();
      //     solution = multigvns.Solve();
      //     update_percentage = multigvns.GetUpdatePercentage();
      //     auto end = std::chrono::steady_clock::now();
      //     performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
      //     break;
      //   }
      //   case 4:
      //     return 0;
    
      //   default:
      //     throw std::invalid_argument("Invalid option.");
      //     break;
      //   }
      //   solution.PrintStudiedSolution(instance, algorithm_name, performance_time.count(), problem.getTasksTimes().size());
      //   std::cout << solution << std::endl << "Performance time: " << performance_time.count() << " seconds" << std::endl << "Update percentage: " << update_percentage << "%" << std::endl;
      // }
    } 
    }
  } catch (std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}