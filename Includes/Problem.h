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
#include <memory>
#include <thread>
#include "Machine.h"

/**
 * @class Problem
 * @brief Class that represents the initial problem
 *        with the machines and the tasks
 */
class Problem {
 public:
  // Método estático para acceder a la instancia global
  static std::shared_ptr<Problem> getInstance(const std::string& file_name = "");
  Problem(std::string file_name) { initialize(file_name); }

  std::vector<Machine> getMachines() const { return machines_; }
  std::vector<Task> getTasksTimes() const { return tasks_times_; }
  int getSetupTimeIn(int i, int j) const { return setup_times_[i][j]; }
  std::vector<std::vector<int>> getSetupTimes() const { return setup_times_; }
  int CalculateSij(int first_task, int second_task) const { return setup_times_[first_task][second_task]; }

  friend std::ostream& operator<<(std::ostream& os, const Problem& problem);
 private:

  Problem() = default;
  void initialize(const std::string& file_name);

  // Evitamos la copia y asignación de la clase
  Problem(const Problem&) = delete;
  Problem& operator=(const Problem&) = delete;

  std::vector<Machine> machines_;
  std::vector<Task> tasks_times_;
  std::vector<std::vector<int>> setup_times_; //TODO: Debería ser una matriz de nxn
};