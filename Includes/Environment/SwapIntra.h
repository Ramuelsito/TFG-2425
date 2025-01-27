/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief SwapIntra class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "Environment.h"

/**
 * @class Intra
 * @brief Class that represents the Intra movements
 *     of the environment
 */
class SwapIntra : public Environment {
 public: 
  SwapIntra() = default;
  SwapIntra(Solution initial_solution, Problem problem) : Environment(initial_solution, problem) {}
  
  Solution GenerateEnvironment() override;
};