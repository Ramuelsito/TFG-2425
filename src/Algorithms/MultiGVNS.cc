/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Multi-GVNS algorithm implementation
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#include "../../Includes/Algorithms/MultiGVNS.h"
#include <random>

/**
 * @brief Solve the problem using the Multi-GVNS algorithm
 * @return The best solution found in i iterations
 */
Solution MultiGVNS::Solve() {
  Solution local_search_solution, shaked_solution, previous_best_solution;
  int iterations_without_improvement = 0;
  ConstructionPhase construction_phase(problem_);
  for (int i = 0; i < 1; ++i) {   
    // current_solution es S, tendremos que mantenerla y 
    // crear otra solucion S' para hacer la busqueda local
    // Entonces guardamos en un fichero S y S'.

    Solution current_solution = construction_phase.ConstructGreedyRandSolution();
    Solution initial_solution = current_solution;
    int k = 1;
    previous_best_solution = best_solution_;
    while (k <= 6) {
      shaked_solution = Shaking(current_solution, k);
      local_search_solution = LocalSearchByVND(shaked_solution);
      if (MoveOrNot(local_search_solution, current_solution)) {
        current_solution = local_search_solution;
        k = 1;
      } else {
        ++k;
      }
    }
    best_solution_ = UpdateSolution(best_solution_, current_solution, initial_solution);
    if (best_solution_.GetTCT() == previous_best_solution.GetTCT()) {
      ++iterations_without_improvement;
    } else {
      iterations_without_improvement = 0;
    }
    if (iterations_without_improvement == 100) {
      break;
    }
  }
  return best_solution_;
}



/**
 * @brief Select a random solution from the enviorenment generated from 
 *     k random ReInsertions Inter.
 * @param k The shaking parameter
 * @return The new solution
 */
Solution MultiGVNS::Shaking(const Solution& initial_solution, const int& k) {
  Solution new_solution;
  Solution current_solution = initial_solution;
  for (int i = 0; i < k; ++i) {
    ReInsertionInter reinsertion_inter(initial_solution, problem_);
    new_solution = reinsertion_inter.SelectRandomNeighbor();
    if (current_solution == new_solution) {        // If the new solution is the same as the current one repeat the selection
      new_solution = reinsertion_inter.SelectRandomNeighbor();
    }
    current_solution = new_solution;
  }
  return new_solution;
}

/**
 * @brief Perform a local search using the VND algorithm
 * @param initial_solution The initial solution
 * @return The local optimum solution
 */
Solution MultiGVNS::LocalSearchByVND(const Solution& initial_solution) {
  Solution local_optimum = initial_solution;
  bool mejora = true;
  while (mejora) {
    ReInsertionIntra reinsertion_intra = ReInsertionIntra(local_optimum, problem_);
    Solution reinsertion_intra_solution = reinsertion_intra.GenerateEnvironment();
    SwapInter swap_inter = SwapInter(reinsertion_intra_solution, problem_);
    Solution swap_inter_solution = swap_inter.GenerateEnvironment();
    SwapIntra swap_intra = SwapIntra(swap_inter_solution, problem_);
    Solution swap_intra_solution = swap_intra.GenerateEnvironment();
    ReInsertionInter reinsertion_inter = ReInsertionInter(swap_intra_solution, problem_);
    Solution reinsertion_inter_solution = reinsertion_inter.GenerateEnvironment();
    if (reinsertion_inter_solution.GetTCT() < local_optimum.GetTCT()) {
      local_optimum = reinsertion_inter_solution;
    } else {
      mejora = false;
    }
  }
  return local_optimum;
}

/**
 * @brief Perform a local search using a random VND algorithm
 * @param initial_solution The initial solution
 * @return The local optimum solution
 */
Solution MultiGVNS::LocalSearchByRandomVND(const Solution& initial_solution) {
  Solution local_optimum = initial_solution;
  bool mejora = true;
  
  std::vector<bool> movements = {true, true, true, true};
  Solution previous_solution = local_optimum;
  std::random_device rd;
  std::mt19937 gen(rd());
  int movement = 0;
  while (mejora) {
    for (int i = 0; i < 4; ++i) {
      // Vector booleano para desactivar temporalmente el entorno
      std::uniform_int_distribution<> dis(0, movements.size() - 1);
      int random_index = dis(gen);
      if (movements[random_index]) { movement = random_index; }
      switch (movement) {
        case 1: {
          ReInsertionIntra reinsertion_intra = ReInsertionIntra(previous_solution, problem_);
          previous_solution = reinsertion_intra.GenerateEnvironment();
          break;
        }
        case 2: {
          SwapInter swap_inter = SwapInter(previous_solution, problem_);
          previous_solution = swap_inter.GenerateEnvironment();
          break;
        }
        case 3: {
          SwapIntra swap_intra = SwapIntra(previous_solution, problem_);
          previous_solution = swap_intra.GenerateEnvironment();
          break;
        }
        case 4: {
          ReInsertionInter reinsertion_inter = ReInsertionInter(previous_solution, problem_);
          previous_solution = reinsertion_inter.GenerateEnvironment();
          break;
        }
      }
    }
    if (previous_solution.GetTCT() < local_optimum.GetTCT()) {
      local_optimum = previous_solution;
    } else {
      mejora = false;
    }
  }
  return local_optimum;
}

Solution MultiGVNS::UpdateSolution(const Solution& current_solution, const Solution& new_solution, const Solution& grasp_solution) {
  if (new_solution.GetTCT() < current_solution.GetTCT()) {
    update_percentage_ = 100 * (grasp_solution.GetTCT() - new_solution.GetTCT()) / grasp_solution.GetTCT();
    return new_solution;
  }
  return current_solution;
}

bool MultiGVNS::MoveOrNot(const Solution& current_solution, const Solution& best_solution) {
  if (current_solution.GetTCT() < best_solution.GetTCT()) {
    return true;
  }
  return false;
}
