/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Instance generator class implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 06 2025  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Instances/Instance.h"
#include <random>
#include <fstream>
#include <filesystem>

/**
 * @brief Instance generator
 * @param k Number of clusters
 * @param correlation Correlation coefficient
 * @return none
 */
void Instance::GenerateInstance(int k, double correlation) {
  task_times_.resize(number_of_tasks_);
  setup_times_ = Matrix(number_of_tasks_ + 1, number_of_tasks_ + 1);
  std::random_device rd;
  std::mt19937 gen(rd());
  // * Generate tasks times with the given distribution
  if (tasks_distribution_ == "uniform") {
    std::uniform_int_distribution<int> dis(limits_.min_task_time, limits_.max_task_time);
    for (int i = 0; i < number_of_tasks_; i++) { 
      task_times_[i] = dis(gen);
    }
  } else if (tasks_distribution_== "normal") {
    std::normal_distribution<> dis(limits_.min_task_time, limits_.max_task_time);
    for (int i = 0; i < number_of_tasks_; i++) { task_times_[i] = static_cast<int>(dis(gen)); }
  } else if (tasks_distribution_== "poisson") {
    std::poisson_distribution<> dis((limits_.min_task_time + limits_.max_task_time) / 2);
    for (int i = 0; i < number_of_tasks_; i++) { task_times_[i] = static_cast<int>(dis(gen)); }
  } else if (tasks_distribution_== "exponential") {
    std::exponential_distribution<> dis(limits_.min_task_time);
    for (int i = 0; i < number_of_tasks_; i++) { task_times_[i] = static_cast<int>(dis(gen)); }
  } else {
    std::cerr << "Distribution not supported" << std::endl;
  }
  std::cout << "Task times: " << std::endl << std::endl;
  // * Generate setup times with different philosophies
  if (setup_distribution_ == "uniform") {
    std::uniform_int_distribution<int> dis(limits_.min_setup_time, limits_.max_setup_time);
    for (int i = 0; i < number_of_tasks_ + 1; i++) {
      for (int j = 0; j < number_of_tasks_ + 1; j++) {
        if (i == j) { continue; }
        setup_times_(i, j) = dis(gen);
      }
    }
  } else if (setup_distribution_== "normal") {
    std::normal_distribution<> dis(limits_.min_setup_time, limits_.max_setup_time);
    for (int i = 0; i < number_of_tasks_ + 1; i++) {
      for (int j = 0; j < number_of_tasks_ + 1; j++) {
        if (i == j) { continue; }
        setup_times_(i, j) = static_cast<int>(dis(gen));
      }
    }
  } else if (setup_distribution_== "clustered") { // * Tries to simulate, that similar tasks have less setup time
    std::uniform_int_distribution<int> dis(limits_.min_setup_time, limits_.max_setup_time);
    int cluster_size = number_of_tasks_ / k;
    for (int i = 0; i < number_of_tasks_ + 1; i++) {
      for (int j = 0; j < number_of_tasks_ + 1; j++) {
        if (i == j) { continue; }
        if (i / cluster_size == j / cluster_size) {
          setup_times_(i, j) = dis(gen) / 2; // In-cluster setup times are smaller
        } else {
          setup_times_(i, j) = dis(gen) * 1.5; // Out-of-cluster setup times are larger
        }
      }
    }
  } else if (setup_distribution_== "correlated") { // * Bigger task times --> bigger setup times
    std::uniform_int_distribution<int> dis(limits_.min_setup_time, limits_.max_setup_time);
    for (int i = 0; i < number_of_tasks_ + 1; i++) {
      for (int j = 0; j < number_of_tasks_ + 1; j++) {
        if (i == j) { continue; }
        int random_component = dis(gen);
        setup_times_(i, j) = correlation * task_times_[i] + (1 - correlation) * random_component;
        setup_times_(i, j) = std::max(limits_.min_setup_time, std::min(limits_.max_setup_time, setup_times_(i, j))); // Ensure setup time is within limits
      }
    }
  } else {
    std::cerr << "Distribution not supported" << std::endl;
  }
}

/**
 * @brief Save the instance to a file
 * @return none
 */
void Instance::SaveInstance() {
  const std::string path = "../Instances/";
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error("Directory [" + path + "] does not exist.");
  }
  char number_of_instance = '1';
  std::string filename = "I" + std::to_string(number_of_tasks_) + "j_" + std::to_string(number_of_machines_) + "m_" + static_cast<char>(toupper(tasks_distribution_[0]))  
                          + "_" + static_cast<char>(toupper(setup_distribution_[0])) + "_" + number_of_instance + ".txt";
  for (const auto& entry : std::filesystem::directory_iterator(path)) { 
    std::string existing_file = entry.path().string();
    if (existing_file.substr(13, 12) == (path + filename).substr(13, 12)) {
      number_of_instance = existing_file.rbegin()[4];
    }
  }
  std::cout << "Number of instance: " << number_of_instance << std::endl;
  number_of_instance = static_cast<char>(number_of_instance + 1);
  filename = "I" + std::to_string(number_of_tasks_) + "j_" + std::to_string(number_of_machines_) + "m_" + static_cast<char>(toupper(tasks_distribution_[0]))  
                          + "_" + static_cast<char>(toupper(setup_distribution_[0])) + "_" + number_of_instance + ".txt";
  std::ofstream file(path + filename);
  if (!file.is_open()) { std::cerr << "Error opening file: " << path + filename << std::endl; return; }
  file << "n: " << number_of_tasks_ << std::endl;
  file << "m: " << number_of_machines_ << std::endl;
  file << "Pi:U[" << limits_.min_task_time << "-" << limits_.max_task_time << "] ";
  for (int i = 0; i < number_of_tasks_; i++) {
    file << task_times_[i] << " ";
  }
  file << std::endl;
  file << "Si:U[" << limits_.min_setup_time << "-" << limits_.max_setup_time << "]" << std::endl;
  for (int i = 0; i < number_of_tasks_ + 1; i++) {
    for (int j = 0; j < number_of_tasks_ + 1; j++) {
      file << setup_times_(i, j) << " ";
    }
    file << std::endl;
  }
  file.close();
  std::cout << "Instance saved as: " << filename << std::endl;
  std::cout << "setup_times_: " << std::endl << setup_times_ << std::endl;
}