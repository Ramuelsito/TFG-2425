/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief SolutionDataTable class implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/FLA/StudiedSolution.h"

/**
 * @brief Write the header of the CSV file
 * @param filename The name of the file to write to
 * @note The file will be opened in truncate mode
 */
void StudiedSolution::WriteHeader(const std::string& filename) const {
  std::ofstream file(filename, std::ios::trunc);
  if (file.is_open()) {
    file << "Instance Name, " 
         << "Min Time, "
         << "Max Time, "
         << "Range," 
         << "Mean, " 
         << "Median, " 
         << "Variance, " 
         << "Standard Deviation, "
         << "Times Used, "
         << "Times Improved, "
         << "Times Used Conditioned, "
         << "Times Improved Conditioned, "
         << "Mean Impacts, "
         << "Max Difference, "
         << "Min Difference, "
         << "Mean Differences, "
         << "Max Size of Walk, "
         << "Min Size of Walk, "
         << "Mean Size of Walk"
         << std::endl;
    file.close();
  } else {
    std::cerr << "Error opening file: " << filename << std::endl;
  }
}

/**
 * @brief Write the data to a CSV file
 * @param filename The name of the file to write to
 * @note The file will be opened in append mode
 */
void StudiedSolution::WriteCSVFile(const std::string& filename) const {
  // Abrir el archivo para añadir una entrada
  std::ofstream file(filename, std::ios::app);
  if (file.is_open()) {
    file << instance_name_ << ",";
    instance_data_->WriteToStream(file);
    file << ",";
    neighborhood_data_->WriteToStream(file);
    file << ",";
    solution_data_table_->WriteToStream(file);
    file << std::endl;
    file.close();
  } else {
    std::cerr << "Error opening file: " << filename << std::endl;
  }
}