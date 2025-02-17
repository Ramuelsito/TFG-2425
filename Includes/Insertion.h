/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Movement struct declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Feb 06 2025  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include "Tasks.h"

struct Insertion {
  Task task;
  int dest_task_index;
  int increment_tct;

  bool operator==(const Insertion& other) const {
    return task == other.task && dest_task_index == other.dest_task_index && increment_tct == other.increment_tct;
  }
  bool operator<(const Insertion& other) const { return increment_tct < other.increment_tct; }
};