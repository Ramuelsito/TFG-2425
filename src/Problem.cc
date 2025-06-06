/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Problem methods implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../Includes/Problem.h"
#include <algorithm>

/**
 *  @brief Get the thread-local instance of the Problem class
 * @param file_name Optional file name to initialize the instance
 * @return A reference to the thread-local Problem instance
 */
Problem& Problem::getInstance(const std::string& file_name) {
  thread_local Problem instance; // Cada hilo tiene su propia instancia
  if (!file_name.empty()) {
    instance.initialize(file_name);
    std::cout << "Problem instance initialized for thread: " << std::this_thread::get_id() << " with file: " << file_name << std::endl;
  }
  return instance;
}

/**
 * Initialize the problem with the input file
 * @param file_name - Name of the file with the problem
 */
void Problem::initialize(const std::string& file_name) {
  if (file_name.empty()) {
    throw std::invalid_argument("File name cannot be empty");
  }
  resetInstance();
  std::ifstream file(file_name);
  if (!file.is_open()) {
    throw std::invalid_argument("File not found");
  }
  char auxiliar_char;
  int machines, tasks;
  while (file >> auxiliar_char) {
    switch (auxiliar_char) {
    case 'n':
      file >> auxiliar_char >> tasks;
      break;

    case 'm':
      file >> auxiliar_char >> machines;
      machines_.reserve(machines);
      for (int i = 0; i < machines; i++) {
        // Machine machine{i};
        machines_.emplace_back(i);
      }
      break;

    case 'P': {
      while (auxiliar_char != ']') {
        file >> auxiliar_char;
      }
      tasks_times_.resize(tasks);
      int task_id = 0;
      int task_time;
      for (Task& actual_task : tasks_times_) {
        actual_task.SetId(task_id);
        file >> task_time;
        actual_task.SetTime(task_time);
        task_id++;
        
      }
      break;
    }

    case 'S': {
      // while (auxiliar_char != ']') {
      //   file >> auxiliar_char;
      // }
      std::string dummy;
      std::getline(file, dummy); // Salta el resto de la línea
      setup_times_.resize(tasks + 1, std::vector<int>(tasks + 1));
      for (std::vector<int>& row : setup_times_) {
        for (int& time : row) {
          file >> time;
        }
      }
      break;
    }

    default:
      std::cout << auxiliar_char << std::endl;
      throw std::invalid_argument("Invalid file format");
    }
  }
  // Here we sort the tasks by their processing time, so the index 
  // doesn't correspond to the task id in the tasks_times_ vector
  // std::sort(tasks_times_.begin(), tasks_times_.end());
  file.close();
}

/**
  * Overload of the << operator to print the problem
  * @param os - Output stream
  * @param problem - Problem to print
  * @return - Output stream
 */
std::ostream& operator<<(std::ostream& os, const Problem& problem) {
  os << "Machines: [" << problem.machines_.size() << "] \n";
  for (Machine machine : problem.machines_) {
    os << machine << std::endl;
  }
  os << "Tasks: [" << problem.tasks_times_.size() << "] ";
  for (Task task : problem.tasks_times_) {
    os << task << " ";
  }
  os << std::endl;
  os << "Setup times: [" << problem.setup_times_.size() << " x " << problem.setup_times_[0].size() << "] " << std::endl;
  for (const std::vector<int>& row : problem.setup_times_) {
    for (int time : row) {
      os << time << " ";
    }
    os << std::endl;
  }
  return os;
}