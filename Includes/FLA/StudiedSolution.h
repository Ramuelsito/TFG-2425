/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Studied solution class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "SolutionDataTable.h"
#include "NeighborhoodData.h"
#include "../Instances/InstanceData.h"

class StudiedSolution {
 public:
  StudiedSolution() = default;
  StudiedSolution(const std::string& instance_name,
                  std::unique_ptr<SolutionDataTable> solution_data_table,
                  std::unique_ptr<NeighborhoodData> neighborhood_data,
                  std::unique_ptr<InstanceData> instance_data)
      : instance_name_(instance_name),
        solution_data_table_(std::move(solution_data_table)),
        neighborhood_data_(std::move(neighborhood_data)),
        instance_data_(std::move(instance_data)) {}

  const SolutionDataTable* GetSolutionDataTable() const { return solution_data_table_.get(); }
  const NeighborhoodData* GetNeighborhoodData() const { return neighborhood_data_.get(); }
  const InstanceData* GetInstanceData() const { return instance_data_.get(); }

  void WriteHeader(const std::string& filename) const;
  void WriteCSVFile(const std::string& filename) const;

 private:
  std::string instance_name_;
  std::unique_ptr<SolutionDataTable> solution_data_table_;
  std::unique_ptr<NeighborhoodData> neighborhood_data_;
  std::unique_ptr<InstanceData> instance_data_;
};