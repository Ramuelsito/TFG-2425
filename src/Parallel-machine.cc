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
#include <thread>
#include <mutex>

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
  thread_local std::unique_ptr<Problem> thread_local_problem;
  if (std::string(argv[1]) == "-all") {
    std::vector<std::thread> threads; // Vector para almacenar los hilos
    std::mutex output_mutex;          // Mutex para proteger la salida estándar
    // ! Hay un segmentation fault en esta parte
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      threads.emplace_back([&, entry]() {
        try {
          // Inicializar Problem específico para este hilo
          thread_local_problem = std::make_unique<Problem>(entry.path().string());
          // TODO: Detectar automaticamente el número de tareas en la instancia 
          MultiGVNS multigvns(40);
          Solution solution = multigvns.Solve();

          // Bloquear el mutex para proteger la salida estándar
          std::lock_guard<std::mutex> lock(output_mutex);
          std::cout << "Instance: " << entry.path().filename() << std::endl;
          std::cout << solution << std::endl;
        } catch (const std::exception& e) {
          // Manejar errores en el hilo
          std::lock_guard<std::mutex> lock(output_mutex);
          std::cerr << "Error processing instance " << entry.path().filename() << ": " << e.what() << std::endl;
        }
      });
    }

    // Esperar a que todos los hilos terminen
    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
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
    std::shared_ptr<Problem> problem = Problem::getInstance("../Instances/" + instance + ".txt");
    std::cout << problem << std::endl;
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
    solution.PrintStudiedSolution(instance, algorithm_name, performance_time.count(), Problem::getInstance()->getTasksTimes().size());
    std::cout << solution << std::endl << "Performance time: " << performance_time.count() << " seconds" << std::endl << "Update percentage: " << update_percentage << "%" << std::endl;
    std::cout << "Neighborhood data: " << std::endl;
    std::cout << multigvns.GetNeighborhoodData() << std::endl;
    std::cout << "Solution data table: " << std::endl;
    std::unique_ptr<SolutionDataTable> solution_table = std::make_unique<SolutionDataTable>(multigvns.GetSolutionDataTable());
    solution_table->PrintTable();
    std::unique_ptr<NeighborhoodData> neighborhood_data = std::make_unique<NeighborhoodData>(multigvns.GetNeighborhoodData());
    InstanceData data;
    std::cout << data << std::endl;
    std::unique_ptr<InstanceData> instance_data = std::make_unique<InstanceData>(data);
    StudiedSolution studied_solution(instance, std::move(solution_table), std::move(neighborhood_data), std::move(instance_data));
    // studied_solution.WriteHeader("../Results/sourceData.csv");
    studied_solution.WriteCSVFile("../Results/sourceData.csv");
  }
  return 0;
}
