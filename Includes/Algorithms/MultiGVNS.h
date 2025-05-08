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
#include "ExhaustedConstructionPhase.h"
#include "../Environment/ReInsertionIntra.h"
#include "../Environment/SwapIntra.h"
#include "../Environment/ReInsertionInter.h"
#include "../Environment/SwapInter.h"
#include "../FLA/NeighborhoodData.h"
#include "../FLA/SolutionDataTable.h"


/**
 * @class MultiGVNS
 * @brief Class that represents the Multi-GVNS algorithm
 */
class MultiGVNS {
 public:
  MultiGVNS(const int& max_iterations) : number_jobs_(max_iterations) {
    best_solution_ = Solution();
    best_solution_.AddTCT(999999);
    update_percentage_ = 0;
    neighborhood_data_ = NeighborhoodData();
  } 
  Solution Solve();
  double GetUpdatePercentage() const { return update_percentage_; }
  const NeighborhoodData& GetNeighborhoodData() const { return neighborhood_data_; }
  const SolutionDataTable& GetSolutionDataTable() const { return solution_data_table_; }

 private: 
  Solution Shaking(const Solution&, const int&);
  Solution LocalSearchByVND(const Solution&);
  Solution LocalSearchByRandomVND(const Solution&, int&);
  void UpdateSolution(const Solution&, const Solution&);

  Solution best_solution_;
  SolutionDataTable solution_data_table_;
  NeighborhoodData neighborhood_data_;
  double update_percentage_;
  int number_jobs_;

  mutable std::mutex data_mutex_; // Mutex para proteger los datos compartidos
};