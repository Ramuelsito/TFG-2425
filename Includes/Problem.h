/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Problem class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Machine.h"

/**
 * @class Problem
 * @brief Class that represents the initial problem
 *        with the machines and the tasks
 */
class Problem {
 public:
  Problem(std::string file_name);
  Problem(std::vector<Machine> machines, std::vector<Task> tasks, std::vector<std::vector<int>> setup_times) :
    machines_(machines), tasks_times_(tasks), setup_times_(setup_times) {}
  
  std::vector<Machine> getMachines() const { return machines_; }
  std::vector<Task> getTasksTimes() const { return tasks_times_; }
  std::vector<std::vector<int>> getSetupTimes() const { return setup_times_; }

  friend std::ostream& operator<<(std::ostream& os, const Problem& problem);
 private:
  std::vector<Machine> machines_;
  std::vector<Task> tasks_times_;
  std::vector<std::vector<int>> setup_times_;
};