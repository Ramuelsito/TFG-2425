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

#include "../Includes/FLA/InstancesGenerator.h"
#include <fstream>

void InstancesGenerator::GenerateInstance() {
  std::ofstream file;
  std::string file_name = "I" + std::to_string(number_of_tasks_) + "_j" + std::to_string(number_of_machines_) + "m_SR.txt";
  file.open(file_name);
  file << "n: " << number_of_tasks_ << std::endl;
  file << "m: " << number_of_machines_ << std::endl;
  file << "Pi:U[" << min_task_time_ << "," << max_task_time_ << "] ";
  for (int i = 0; i < number_of_tasks_; i++) {
    file << rand() % (max_task_time_ - min_task_time_ + 1) + min_task_time_ << " ";
  }
  file << std::endl;
  for (int i = 0; i < number_of_tasks_; i++) {
    for (int j = 0; j < number_of_tasks_; j++) {
      if (i == j) {
        file << "0 ";
        continue;
      }
      file << rand() % (max_setup_time_ - min_setup_time_ + 1) + min_setup_time_ << " ";
    }
    file << std::endl;
  }
  file.close();
}