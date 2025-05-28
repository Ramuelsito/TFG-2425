/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief NeighborhoodData class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 22 2025
 * @see {https://github.com/alu0101471136}
 */

#pragma once

#include "Matrix.h"

class NeighborhoodData {
 public:
  NeighborhoodData() {
    // Size --> 4, 1 por cada movimiento
    times_used_.resize(4, 0);
    times_improved_.resize(4, 0);
    // Size --> 4 x 4, condicionada por cada movimiento
    times_used_condicioned_ = Matrix(5, 4);
    times_improved_condicioned_ = Matrix(5, 4);
  }
  
  void UpdateTimesUsed(int index) { times_used_[index]++; }
  void UpdateTimesImproved(int index) { times_improved_[index]++; }
  void UpdateTimesUsedConditioned(int index1, int index2) { times_used_condicioned_(index1, index2)++; }
  void UpdateTimesImprovedConditioned(int index1, int index2) { times_improved_condicioned_(index1, index2)++; }

  friend std::ostream& operator<<(std::ostream& os, const NeighborhoodData& neighborhood_data) {
    os << "Times used: " << std::endl;
    for (int i = 0; i < neighborhood_data.times_used_.size(); ++i) {
      os << neighborhood_data.times_used_[i] << " ";
    }
    os << std::endl;
    os << "Times improved: " << std::endl;
    for (int i = 0; i < neighborhood_data.times_improved_.size(); ++i) {
      os << neighborhood_data.times_improved_[i] << " ";
    }
    os << std::endl;
    os << "Times used conditioned: " << std::endl;
    os << neighborhood_data.times_used_condicioned_ << std::endl;
    os << "Times improved conditioned: " << std::endl;
    os << neighborhood_data.times_improved_condicioned_ << std::endl;
    return os;
  }

  void WriteToStream(std::ostream& os) const {
    for (int i = 0; i < times_used_.size(); ++i) {
      os << times_used_[i] << " ";
    }
    os << ",";
    for (int i = 0; i < times_improved_.size(); ++i) {
      os << times_improved_[i] << " ";
    }
    os << ",\"" 
       << times_used_condicioned_;
    // times_used_condicioned_.WriteInline(os);
    os << "\",\""
       << times_improved_condicioned_;
    // times_improved_condicioned_.WriteInline(os);
    os << "\"";
  }
 private:
  std::vector<int> times_used_;
  std::vector<int> times_improved_;
  Matrix times_used_condicioned_;
  Matrix times_improved_condicioned_;
};