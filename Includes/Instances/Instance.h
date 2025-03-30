/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Instance generator class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 06 2025  
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "../FLA/Matrix.h"

struct TimeLimits {
  int min_task_time;
  int max_task_time;
  int min_setup_time;
  int max_setup_time;
};

class Instance {
 public:
  Instance(int number_of_tasks, int number_of_machines, std::string tasks_distribution, std::string setup_distribution, int max_task_time = 99, int min_task_time = 1, int max_setup_time = 49, int min_setup_time = 1) :
          number_of_tasks_(number_of_tasks), number_of_machines_(number_of_machines), tasks_distribution_(tasks_distribution), setup_distribution_(setup_distribution) {
    limits_.min_task_time = min_task_time;
    limits_.max_task_time = max_task_time;
    limits_.min_setup_time = min_setup_time;
    limits_.max_setup_time = max_setup_time;
  }
  ~Instance() = default;
  void GenerateInstance(int k = 4, double correlation = 0.5);
  void SaveInstance();
 private:
  int number_of_tasks_;
  int number_of_machines_;
  std::vector<int> task_times_;
  Matrix setup_times_;
  
  TimeLimits limits_;
  std::string tasks_distribution_;
  std::string setup_distribution_;
};