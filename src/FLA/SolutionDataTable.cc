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

#include "../../Includes/FLA/SolutionDataTable.h"

/**
 * @brief Calculate the mean, max, min, and size of the solution data table
 * @param total_impacts The mean of the impacts
 * @param total_max The total max of the differences
 * @param total_min The total min of the differences
 * @param total_mean_differences The total mean of the differences
 * @param total_max_size The total max size of the walk
 * @param total_min_size The total min size of the walk
 * @param total_mean_size The total mean size of the walk
 * @note The function iterates over the table once, if we do this for each
 *       solution, it will be much slower
 */
void SolutionDataTable::AnalizeTable(double& total_impacts, double& total_max, double& total_min, double& total_mean_differences, 
                                     double& total_max_size, double& total_min_size, double& total_mean_size) const {
  for (const auto& [solution, data] : table_) {
    total_impacts += data.impacts_;
    total_max = (total_max > data.max_differences_objective_function_) ? total_max : data.max_differences_objective_function_;
    total_min = (total_min < data.min_differences_objective_function_) ? total_min : data.min_differences_objective_function_;
    total_mean_differences += data.GetMeanDifferences();
    total_max_size = (total_max_size > data.max_size_of_walk_) ? total_max_size : data.max_size_of_walk_;
    total_min_size = (total_min_size < data.min_size_of_walk_) ? total_min_size : data.min_size_of_walk_;
    total_mean_size += data.GetMeanSizeOfWalk();
  }  
  total_mean_differences /= total_impacts;
  total_mean_size /= total_impacts;
} 

/**
 * @brief Write the data to a CSV file
 * @param filename The name of the file to write to
 * @note The file will be opened in append mode
 */
void SolutionDataTable::WriteToStream(std::ostream& os) const {
  double total_impacts = 0;
  double total_max = -1;
  double total_min = __INT16_MAX__;
  double total_mean_differences = 0;
  double total_max_size = -1;
  double total_min_size = __INT16_MAX__;
  double total_mean_size = 0;
  AnalizeTable(total_impacts, total_max, total_min, total_mean_differences, total_max_size, total_min_size, total_mean_size);

  os << total_impacts << ","
     << total_max << ","
     << total_min << ","
     << total_mean_differences << ","
     << total_max_size << ","
     << total_min_size << ","
     << total_mean_size;
}