/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief SwapInter class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "Environment.h"

/**
 * @class SwapInter
 * @brief Class that represents the Inter swap
 */
class SwapInter : public Environment {
 public: 
  SwapInter() = default;
  SwapInter(const Solution& initial_solution, const Problem& problem) : Environment(initial_solution, problem) {}
  
  Solution GenerateEnvironment() override;
};