/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief NeighborhoodData class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "SolutionData.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <numeric>
#include <iostream>

class SolutionDataTable {
 public:
  // Agregar una nueva entrada a la tabla
  void AddSolutionData(const Solution& solution, int difference, int size_of_walk) {
    auto& data = table_[solution];
    data.Update(difference, size_of_walk);
  }

    // Imprimir la tabla
  void PrintTable() const {
    for (const auto& [solution, data] : table_) {
      std::cout << "Solution: " << solution << std::endl;
      std::cout << "  Impacts: " << data.impacts_ << std::endl;
      std::cout << "  Max Differences: " << data.max_differences_objective_function_ << std::endl;
      std::cout << "  Min Differences: " << data.min_differences_objective_function_ << std::endl;
      std::cout << "  Mean Differences: " << data.GetMeanDifferences() << std::endl;
      std::cout << "  Max Size of Walk: " << data.max_size_of_walk_ << std::endl;
      std::cout << "  Min Size of Walk: " << data.min_size_of_walk_ << std::endl;
      std::cout << "  Mean Size of Walk: " << data.GetMeanSizeOfWalk() << std::endl;
      std::cout << "----------------------------------------" << std::endl;
    }
  }

  void AnalizeTable(int& number_diff_local_optimums, double& total_impacts, double& total_max, double& total_min, 
                    double& total_mean_differences, double& total_max_size, double& total_min_size, double& total_mean_size, 
                    double& min_tct, double& max_tct, double& mean_tct) const;
  void WriteToStream(std::ostream& os) const;
 private:
  std::unordered_map<Solution, SolutionData> table_;
};