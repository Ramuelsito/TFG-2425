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
#include <algorithm>

/**
 * @brief Solve the problem using the Multi-GVNS algorithm
 * @return The best solution found in i iterations
 */
Solution MultiGVNS::Solve() {
  std::unique_ptr<Solution> local_search_solution, shaked_solution, previous_best_solution, current_solution, initial_solution;
  int iterations_without_improvement = 0;
  ExhaustedConstructionPhase construction_phase;
  
  for (int iteration = 0; iteration < 5; iteration++) {        //! 5 iteraciones
    iterations_without_improvement = 0;
    for (int i = 0; i < number_jobs_ * number_jobs_; ++i) {    //! n^2 iteraciones
      int difference = 0;
      int size_of_walk = 0;
      current_solution = std::make_unique<Solution>(construction_phase.ConstructGreedyRandSolution());
      initial_solution = std::make_unique<Solution>(*current_solution);
      
      int k = 1;
      previous_best_solution = std::make_unique<Solution>(best_solution_);
      while (k <= 6) {
        std::unique_ptr<Solution> shaked_solution = std::make_unique<Solution>(Shaking(*current_solution, k));
        std::unique_ptr<Solution> local_search_solution = std::make_unique<Solution>(LocalSearchByRandomVND(*shaked_solution, size_of_walk));
        if (local_search_solution->GetTCT() < current_solution->GetTCT()) {
          current_solution = std::move(local_search_solution);
          if (!current_solution) {
            throw std::runtime_error("current_solution is not initialized");
          }
          k = 1;
        } else {
          ++k;
        }
      }
      difference = abs(current_solution.get()->GetTCT() - initial_solution.get()->GetTCT());
      solution_data_table_.AddSolutionData(*current_solution, difference, size_of_walk);
      UpdateSolution(*current_solution, *initial_solution);
      // std::cout << "Best solution: " << best_solution_.GetTCT() << std::endl;
      // std::cout << "Current solution: " << current_solution->GetTCT() << std::endl;
      // std::cout << "Iterations without improvement: " << iterations_without_improvement << std::endl;
      if (best_solution_.GetTCT() == previous_best_solution->GetTCT()) {
        ++iterations_without_improvement;
      } else {
        iterations_without_improvement = 0;
      }
      if (iterations_without_improvement == 300) { break; }
    }
    construction_phase.IncreaseAlpha();
  }
  return best_solution_;
}

// Solution MultiGVNS::Solve() { // ! Sin punteros inteligentes
//   Solution local_search_solution, shaked_solution, previous_best_solution;
//   int iterations_without_improvement = 0;
//   ExhaustedConstructionPhase construction_phase;
//   for (int alpha_inc = 0; alpha_inc < 5; alpha_inc++) {        //! 5 iteraciones
//     iterations_without_improvement = 0;
//     for (int i = 0; i < number_jobs_ * number_jobs_; ++i) {    //! n^2 iteraciones
//       int difference = 0;
//       int size_of_walk = 0;
      
//       // current_solution es S, tendremos que mantenerla y 
//       // crear otra solucion S' para hacer la busqueda local
//       // Entonces guardamos en un fichero S y S'.
//       Solution current_solution = construction_phase.ConstructGreedyRandSolution();
//       Solution initial_solution = current_solution;
//       // std::cout << "Initial solution: " << std::endl;
//       // std::cout << current_solution << std::endl;
  
//       int k = 1;
//       previous_best_solution = best_solution_;
//       while (k <= 6) {
//         shaked_solution = Shaking(current_solution, k);
//         local_search_solution = LocalSearchByRandomVND(shaked_solution, size_of_walk);
//         if (local_search_solution.GetTCT() < current_solution.GetTCT()) {
//           current_solution = local_search_solution;
//           k = 1;
//         } else {
//           ++k;
//         }
//       }
//       difference = abs(current_solution.GetTCT() - initial_solution.GetTCT());
//       solution_data_table_.AddSolutionData(current_solution, difference, size_of_walk);
//       UpdateSolution(current_solution, initial_solution);
//       if (best_solution_.GetTCT() == previous_best_solution.GetTCT()) {
//         ++iterations_without_improvement;
//       } else {
//         iterations_without_improvement = 0;
//       }
//       if (iterations_without_improvement == 300) { break; }
//     }
//     construction_phase.IncreaseAlpha();
//   }
//   return best_solution_;
// }


/**
 * @brief Select a random solution from the enviorenment generated from 
 *     k random ReInsertions Inter.
 * @param k The shaking parameter
 * @return The new solution
 */
Solution MultiGVNS::Shaking(const Solution& initial_solution, const int& k) {
  Solution new_solution = initial_solution;
  Solution current_solution = initial_solution;
  for (int i = 0; i < k; ++i) {
    ReInsertionInter reinsertion_inter(new_solution);
    new_solution = reinsertion_inter.SelectRandomNeighbor();
    if (current_solution == new_solution) { new_solution = reinsertion_inter.SelectRandomNeighbor(); } // If the new solution is the same as the current one repeat the selection
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
  Solution current_solution = local_optimum;
  bool mejora = true;
  while (mejora) {
    ReInsertionIntra reinsertion_intra = ReInsertionIntra(current_solution);
    current_solution = reinsertion_intra.GenerateEnvironment();
    SwapInter swap_inter = SwapInter(current_solution);
    current_solution = swap_inter.GenerateEnvironment();
    SwapIntra swap_intra = SwapIntra(current_solution);
    current_solution = swap_intra.GenerateEnvironment();
    // Esta en el reinsert inter
    ReInsertionInter reinsertion_inter = ReInsertionInter(current_solution);
    current_solution = reinsertion_inter.GenerateEnvironment();
    if (current_solution.GetTCT() < local_optimum.GetTCT()) {
      local_optimum = current_solution;
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
Solution MultiGVNS::LocalSearchByRandomVND(const Solution& initial_solution, int& size_of_walk) {
  Solution local_optimum = initial_solution;
  bool mejora = true;
  std::vector<int> available_movements = {0, 1, 2, 3};
  Solution previous_solution;
  std::random_device rd;
  std::mt19937 gen(rd());
  int movement = 0;
  int previous_movement = -1;
  while (mejora) {
    previous_solution = local_optimum;
    std::uniform_int_distribution<> dis(0, available_movements.size() - 1);
    int index = dis(gen);
    movement = available_movements[index];
    // Tenemos que apuntar las veces que arranca y mejora, Rate successfully improved
    switch (movement) {
      case 0: {
        // std::cout << "Movement Reinsertion intra ";
        ReInsertionIntra reinsertion_intra = ReInsertionIntra(previous_solution);
        previous_solution = reinsertion_intra.GenerateEnvironment();
        break;
      }
      case 1: {
        // std::cout << "Movement Swap inter ";
        SwapInter swap_inter = SwapInter(previous_solution);
        previous_solution = swap_inter.GenerateEnvironment();
        break;
      }
      case 2: {
        // std::cout << "Movement Swap intra ";
        SwapIntra swap_intra = SwapIntra(previous_solution);
        previous_solution = swap_intra.GenerateEnvironment();
        break;
      }
      case 3: {
        // std::cout << "Movement Reinsertion inter ";
        ReInsertionInter reinsertion_inter = ReInsertionInter(previous_solution);
        previous_solution = reinsertion_inter.GenerateEnvironment();
        break;
      }
    }
    // ! Cuando se usa un movimiento por primera vez, no se almacena en las matrices condicionadas
    // ! Lo que puede dar lugar a que no coincida las veces que se usa un movimiento con las veces que se usa condicionado
    if (previous_solution.GetTCT() < local_optimum.GetTCT()) { // Si mejora
      neighborhood_data_.UpdateTimesImproved(movement);
      neighborhood_data_.UpdateTimesUsed(movement);
      if (previous_movement != -1) { neighborhood_data_.UpdateTimesUsedConditioned(previous_movement, movement); }
      if (previous_movement != -1) { neighborhood_data_.UpdateTimesImprovedConditioned(previous_movement, movement); }
      local_optimum = previous_solution;
      available_movements = {0, 1, 2, 3};
      size_of_walk++;
    } else {
      neighborhood_data_.UpdateTimesUsed(movement);
      if (previous_movement != -1) { neighborhood_data_.UpdateTimesUsedConditioned(previous_movement, movement); }
      available_movements.erase(available_movements.begin() + index);
      if (available_movements.empty()) { mejora = false; }
    }
    previous_movement = movement;
  }
  return local_optimum;
}

void MultiGVNS::UpdateSolution(const Solution& new_solution, const Solution& grasp_solution) {
  std::lock_guard<std::mutex> lock(data_mutex_);
  if (new_solution.GetTCT() < best_solution_.GetTCT()) {
    update_percentage_ = 100 * (grasp_solution.GetTCT() - new_solution.GetTCT()) / grasp_solution.GetTCT();
    // best_solution_ = std::move(new_solution);
    best_solution_ = new_solution;
  }
}
