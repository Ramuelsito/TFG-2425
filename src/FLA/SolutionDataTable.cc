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
void SolutionDataTable::AnalizeTable(int& number_diff_local_optimums, double& total_impacts, double& total_max, double& total_min, double& total_mean_differences, 
                                     double& total_max_size, double& total_min_size, double& total_mean_size, 
                                     double& min_tct, double& max_tct, double& mean_tct) const {
  // The number of different local optimums is equal to the size of the table,
  // since each solution is a different local optimum
  number_diff_local_optimums = table_.size();
  for (const auto& [solution, data] : table_) {
    total_impacts += data.impacts_;
    total_max = (total_max > data.max_differences_objective_function_) ? total_max : data.max_differences_objective_function_;
    total_min = (total_min < data.min_differences_objective_function_) ? total_min : data.min_differences_objective_function_;
    total_mean_differences += data.GetMeanDifferences();
    total_max_size = (total_max_size > data.max_size_of_walk_) ? total_max_size : data.max_size_of_walk_;
    total_min_size = (total_min_size < data.min_size_of_walk_) ? total_min_size : data.min_size_of_walk_;
    total_mean_size += data.GetMeanSizeOfWalk();
    min_tct = (min_tct < solution.GetTCT()) ? min_tct : solution.GetTCT();
    max_tct = (max_tct > solution.GetTCT()) ? max_tct : solution.GetTCT();
    mean_tct += solution.GetTCT();
  }  
  total_mean_differences /= total_impacts;
  total_mean_size /= total_impacts;
  mean_tct /= number_diff_local_optimums;
} 

/**
 * @brief Write the data to a CSV file
 * @param filename The name of the file to write to
 * @note The file will be opened in append mode
 */
void SolutionDataTable::WriteToStream(std::ostream& os) const {
  int number_diff_local_optimums = 0;
  double total_impacts = 0;
  double total_max = -1;
  double total_min = __INT16_MAX__;
  double total_mean_differences = 0;
  double total_max_size = -1;
  double total_min_size = __INT16_MAX__;
  double total_mean_size = 0;
  double min_tct = __INT16_MAX__;
  double max_tct = -1;
  double mean_tct = 0;
  AnalizeTable(number_diff_local_optimums, total_impacts, total_max, total_min, 
               total_mean_differences, total_max_size, total_min_size, total_mean_size, 
               min_tct, max_tct, mean_tct);

  os << number_diff_local_optimums << ","
     << total_impacts << ","
     << total_max << ","
     << total_min << ","
     << total_mean_differences << ","
     << total_max_size << ","
     << total_min_size << ","
     << total_mean_size << ","
     << min_tct << ","
     << max_tct << ","
     << mean_tct;
}