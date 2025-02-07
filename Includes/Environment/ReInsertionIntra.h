/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief ReInsertionIntra class declaration
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
class ReInsertionIntra : public Environment {
 public: 
  ReInsertionIntra() = default;
  ReInsertionIntra(const Solution& initial_solution, const Problem& problem) : Environment(initial_solution, problem) {}
  
  Solution GenerateEnvironment() override;
};