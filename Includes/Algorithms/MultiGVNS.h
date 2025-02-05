/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Multi-GVNS algorithm declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "GRASP-Algorithms/GRASP-min.h"

/**
 * @class MultiGVNS
 * @brief Class that represents the Multi-GVNS algorithm
 */
class MultiGVNS : public Algorithm {
 public:
  MultiGVNS(Problem problem) : problem_(problem) {
    best_solution_.AssignMaxTCT();
    update_percentage_ = 0;
  }
  Solution Solve() override;
  double GetUpdatePercentage() const { return update_percentage_; }
 private: 
  Solution Shaking(const Solution&, const int&);
  Solution LocalSearchByVND(const Solution&);
  Solution LocalSearchByRandomVND(const Solution&);
  Solution UpdateSolution(const Solution&, const Solution&, const Solution&);
  bool MoveOrNot(const Solution&, const Solution&);
  
  Problem problem_;
  Solution best_solution_;
  double update_percentage_;
};