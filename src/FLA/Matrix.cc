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

#include "../../Includes/FLA/Matrix.h"
#include <algorithm>

/**
 * @brief Calculate the variance of the matrix
 * @param mean - Mean of the matrix
 * @return Variance of the matrix
 */
double Matrix::CalculateVariance(const double& mean) const {
  double variance = 0;
  for (int value : data_) {
    variance += (value - mean) * (value - mean); // es lo mismo que std::pow(value - mean, 2);
  }
  return variance / (rows_ * cols_);
}

/**
 * @brief Calculate the median of the matrix
 * @return Median of the matrix
 */
int Matrix::CalculateMedian() const {
  std::vector<int> data = data_;
  std::sort(data.begin(), data.end());
  int middle = data.size() / 2;
  if (data.size() % 2 == 0) {
    // Si el tamaño es par, devolver el promedio de los dos valores centrales
    return (data[middle - 1] + data[middle]) / 2;
  } else {
    // Si el tamaño es impar, devolver el valor central
    return data[middle];
  }
}