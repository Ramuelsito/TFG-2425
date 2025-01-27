/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief ReInsertionInter class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "Environment.h"

/**
 * @class Intra
 * @brief Class that represents the ReInsetion move
 *     inter-machines 
 */
class ReInsertionInter : public Environment {
 public: 
  ReInsertionInter() = default;
  ReInsertionInter(Solution initial_solution, Problem problem) : Environment(initial_solution, problem) {}
  
  Solution GenerateEnvironment() override;
  Solution SelectRandomNeighbor();
};