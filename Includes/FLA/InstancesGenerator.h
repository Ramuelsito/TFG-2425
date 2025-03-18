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

class InstancesGenerator {
 public:
  InstancesGenerator(int number_of_tasks, int number_of_machines, int max_task_time = 99, int min_task_time = 1, int max_setup_time = 49, int min_setup_time = 1) :
    number_of_tasks_(number_of_tasks), number_of_machines_(number_of_machines), max_task_time_(max_task_time), min_task_time_(min_task_time), max_setup_time_(max_setup_time), min_setup_time_(min_setup_time) {}
  ~InstancesGenerator();
  void GenerateInstance();
 private:
  int number_of_tasks_;
  int number_of_machines_;
  int max_task_time_;
  int min_task_time_;
  int max_setup_time_;
  int min_setup_time_;
};