/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief InstanceData class implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Instances/InstanceData.h"
#include "../../Includes/Problem.h"
#include <cmath>

InstanceData::InstanceData() {
  Problem& problem = Problem::getInstance();
  int number_of_tasks = problem.getSetupTimes().size();
  compiled_times_ = Matrix(number_of_tasks, number_of_tasks);
  int max_time = -1;
  int min_time = 999999;
  double mean = 0; 
  int dispher = 0;
  for (int i = 0; i < number_of_tasks; i++) {
    dispher = 1;
    for (int j = 0; j < number_of_tasks; j++) {
      if (i == j) {
        compiled_times_(i, j) = problem.getSetupTimes()[i][j];
        dispher = 2;
      } else if (j == 0 || i == 0) {
        compiled_times_(i, j) = problem.getSetupTimes()[i][j];
      } else {
        compiled_times_(i, j) = problem.getSetupTimeIn(i, j) + problem.getTasksTimes()[j - dispher].GetTime();
        mean += compiled_times_(i, j);
      }
      if (compiled_times_(i, j) > max_time) { max_time = compiled_times_(i, j); }
      if (compiled_times_(i, j) < min_time && compiled_times_(i, j) != 0) { min_time = compiled_times_(i, j); }
    }
  }
  number_of_tasks_ = problem.getTasksTimes().size();
  number_of_machines_ = problem.getMachines().size();
  tasks_machines_proportion_ = number_of_tasks / number_of_machines_;
  min_time_ = min_time;
  max_time_ = max_time;
  range_ = max_time - min_time;
  mean_ = mean / (number_of_tasks * number_of_tasks);
  variance_ = compiled_times_.CalculateVariance(mean_);
  median_ = compiled_times_.CalculateMedian();
  standard_deviation_ = std::sqrt(variance_);
}

void InstanceData::WriteToStream(std::ostream& os) const {
  os << number_of_tasks_ << ","
     << number_of_machines_ << ","
     << tasks_machines_proportion_ << ","
     << min_time_ << "," 
     << max_time_ << "," 
     << range_ << "," 
     << mean_ << "," 
     << median_ << "," 
     << variance_ << "," 
     << standard_deviation_;
}