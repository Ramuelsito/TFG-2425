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
  SwapIntra(const Solution& initial_solution) : Environment(initial_solution) {}
  
  Solution GenerateEnvironment() override;
};