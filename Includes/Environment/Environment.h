/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Environment class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Apr 08 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <iostream>
#include "../Solution.h"
#include "../Problem.h"

/**
 * @class Environment
 * @brief Class that represents the Environment
 */
class Environment{
 public: 
  Environment() = default;
  Environment(Solution initial_solution, Problem problem) : initial_solution_{initial_solution}, problem_{problem} {}
  
  virtual Solution GenerateEnvironment() = 0;
 protected:
  Problem problem_;
  Solution initial_solution_;
  std::vector<Solution> local_optimums_;
};