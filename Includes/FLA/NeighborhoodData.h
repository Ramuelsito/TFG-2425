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

#include "Matrix.h"

class NeighborhoodData {
 public:
  
 private:
  std::vector<int> times_used_;
  std::vector<int> times_improved_;
  Matrix times_used_condicioned_;
  Matrix times_improved_condicioned_;
};