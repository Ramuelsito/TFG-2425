/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Matriz class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include <iostream>
#include <vector>

class Matrix {
 public:
  Matrix(unsigned rows, unsigned cols) {
    rows_ = rows;
    cols_ = cols;
    data_.resize(rows * cols);
  }
  Matrix() = default;

  int& operator()(unsigned row, unsigned col) { return data_[row * cols_ + col]; }
  const int& operator()(unsigned row, unsigned col) const { return data_[row * cols_ + col]; }
  double CalculateVariance(const double& mean) const;
  int CalculateMedian() const;

  friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (int i = 0; i < matrix.rows_; ++i) {
      for (int j = 0; j < matrix.cols_; ++j) {
        os << matrix(i, j) << " ";
      }
      os << std::endl;
    }
    return os;
  }
 private:
  std::vector<int> data_;
  int rows_;
  int cols_;
};