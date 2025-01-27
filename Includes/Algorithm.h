/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Algorithm abstract class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "Problem.h"
#include "Solution.h"

/**
 * @class Algorithm
 * @brief Abstract class that represents an algorithm
 */
class Algorithm {
 public:
  Algorithm() = default;
  ~Algorithm() = default;
  virtual Solution Solve() = 0;
};