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
#include <queue>
#include <condition_variable>
#include <atomic>

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
  if (std::string(argv[1]) == "-threads") {
    const unsigned int max_threads = std::thread::hardware_concurrency();
    std::mutex output_mutex, file_mutex, queue_mutex;
    std::condition_variable cv;
    std::queue<std::filesystem::directory_entry> task_queue;
    std::atomic<bool> done{false};

    // Rellenar la cola de tareas
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      task_queue.push(entry);
    }

    auto worker = [&]() {
      while (true) {
        std::filesystem::directory_entry entry;
        {
          std::unique_lock<std::mutex> lock(queue_mutex);
          cv.wait(lock, [&]{ return !task_queue.empty() || done; });
          if (task_queue.empty()) return;
          entry = task_queue.front();
          task_queue.pop();
        }
        try {
          for (int run = 0; run < 5; ++run) { // Solucionar 5 veces por instancia
            {
              std::lock_guard<std::mutex> lock(output_mutex);
              std::cout << "Processing instance: " << entry.path().filename() << std::endl;
            }
            Problem& problem = Problem::getInstance(entry.path().string());
            int number_of_tasks = problem.getTasksTimes().size();
            MultiGVNS multigvns(number_of_tasks);
  
            auto start = std::chrono::steady_clock::now();
            Solution solution = multigvns.Solve();
            auto end = std::chrono::steady_clock::now();
            double update_percentage = multigvns.GetUpdatePercentage();
            std::chrono::seconds performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
  
            std::string instance = entry.path().filename();
            solution.PrintStudiedSolution(instance, "GVNS", performance_time.count(), Problem::getInstance().getTasksTimes().size());
            std::unique_ptr<SolutionDataTable> solution_table = std::make_unique<SolutionDataTable>(multigvns.GetSolutionDataTable());
            std::unique_ptr<NeighborhoodData> neighborhood_data = std::make_unique<NeighborhoodData>(multigvns.GetNeighborhoodData());
            InstanceData data;
            std::unique_ptr<InstanceData> instance_data = std::make_unique<InstanceData>(data);
  
            StudiedSolution studied_solution(instance, std::move(solution_table), std::move(neighborhood_data), std::move(instance_data));
            {
              std::lock_guard<std::mutex> lock(file_mutex);
              studied_solution.WriteCSVFile("../Results/sourceData.csv");
            }
          }
        } catch (const std::exception& e) {
          std::lock_guard<std::mutex> lock(output_mutex);
          std::cerr << "Error processing instance " << entry.path().filename() << ": " << e.what() << std::endl;
        }
      }
    };

    // Lanzar el pool de hilos
    std::vector<std::thread> thread_pool;
    for (unsigned int i = 0; i < max_threads; ++i) {
      thread_pool.emplace_back(worker);
    }

    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      done = true;
    }
    cv.notify_all();

    for (auto& t : thread_pool) {
      if (t.joinable()) t.join();
    }
  } else if (std::string(argv[1]) == "-all") {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      std::cout << entry.path().string() << std::endl;
      Problem& problem = Problem::getInstance(entry.path().string());
      int number_of_tasks = problem.getTasksTimes().size();
      std::string instance = entry.path().filename();
      instance = instance.substr(0, instance.find_last_of('.'));
      std::cout << instance << "  " << number_of_tasks << std::endl;
      MultiGVNS multigvns(number_of_tasks);
      auto start = std::chrono::steady_clock::now();
      Solution solution = multigvns.Solve();
      auto end = std::chrono::steady_clock::now();
      std::string algorithm_name;
      std::chrono::seconds performance_time;
      double update_percentage;
      update_percentage = multigvns.GetUpdatePercentage();
      performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
      solution.PrintStudiedSolution(instance, algorithm_name, performance_time.count(), Problem::getInstance().getTasksTimes().size());
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
  } else if (std::string(argv[1]) == "-gen") {
    const int kNumInstances = 5;
    int setup_id = 1;
    std::string tasks_distribution = "uniform";
    std::vector<std::string> setup_distribution = {"uniform", "clustered"};
    std::vector<int> machine_options = {2, 4, 6, 8};
    std::vector<int> tasks_options = {40, 50, 70, 80}; 
    std::vector<std::pair<int, int>> task_ranges = { {1, 49}, {1, 99}, {1, 124} };

    // 2 rangos de valores para las tareas, 2 variantes de generacion, 5 instancias de cada combinación. 4 combinaciones de tarea, 4 combinaciones de máquina.
    for (int i = 0; i < kNumInstances; i++) { // 5 instancias
      for (int j = 0; j < machine_options.size(); j++) {
        for (int k = 0; k < tasks_options.size(); k++) {
          for (const auto& range : task_ranges) {
            for (const auto& setup : setup_distribution) {
              Instance instance(tasks_options[k], machine_options[j], setup_id, tasks_distribution, setup, 99, 1, range.second, range.first);
              instance.GenerateInstance(tasks_options[k] / 10, 0.7); // number of clusters, correlation
              instance.SaveInstance();
            }
            ++setup_id;
          }
          setup_id = 1;
        }
      }
    }
  } else {
    std::string instance = argv[1];
    Problem& problem = Problem::getInstance("../Instances/" + instance + ".txt");
    int number_of_tasks = problem.getTasksTimes().size();
    std::cout << problem << std::endl;
    for (int i = 0; i < 10; ++i) {
      Solution solution;
      std::string algorithm_name;
      std::chrono::seconds performance_time;
      double update_percentage;
      algorithm_name = "GVNS";
      MultiGVNS multigvns(number_of_tasks);
      auto start = std::chrono::steady_clock::now();
      solution = multigvns.Solve();
      auto end = std::chrono::steady_clock::now();
      update_percentage = multigvns.GetUpdatePercentage();
      performance_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
      solution.PrintStudiedSolution(instance, algorithm_name, performance_time.count(), Problem::getInstance().getTasksTimes().size());
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
  }
  return 0;
}
