/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief InstanceData class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "../FLA/Matrix.h"

class InstanceData {
 public:
  InstanceData();

  friend std::ostream& operator<<(std::ostream& os, const InstanceData& instance_data) {
    os << "Compiled times: " << std::endl;
    instance_data.compiled_times_.WriteStyled();
    os << "Min time: " << instance_data.min_time_ << std::endl;
    os << "Max time: " << instance_data.max_time_ << std::endl;
    os << "Range: " << instance_data.range_ << std::endl;
    os << "Mean: " << instance_data.mean_ << std::endl;
    os << "Median: " << instance_data.median_ << std::endl;
    os << "Variance: " << instance_data.variance_ << std::endl;
    os << "Standard deviation: " << instance_data.standard_deviation_ << std::endl;
    return os;
  }

  void WriteToStream(std::ostream& filename) const;
 private:
  Matrix compiled_times_;
  int number_of_tasks_;
  int number_of_machines_;
  int tasks_machines_proportion_;
  int min_time_;
  int max_time_;
  int range_;
  double mean_;
  double median_;
  double variance_;
  double standard_deviation_;
};

/**
 * Tendría que calcular el tiempo completo de toda la instancia, que sera T = tiempo de setup + tiempo de ejecución
 * Min(T) de la instancia
 * Max(T) de la instancia
 * Rango de T (Resta de Max(T) - Min(T))
 * Media de T
 * Mediana de T
 * Varianza de T
 * Desviación típica de T
 */