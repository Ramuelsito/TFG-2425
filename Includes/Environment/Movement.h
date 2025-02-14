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

/**
 * @struct Movement
 * @brief Struct that represents a movement in the environment
 * @param orig_machine_index - Original machine index
 * @param orig_task_index - Original task index, in the original machine
 * @param dest_machine_index - Destination machine index
 * @param dest_task_index - Destination task index, in the destination machine
 * @param tct_increment - Increment of the total completion time
 */
struct Movement {
  int orig_machine_index;
  int orig_task_index;
  int dest_machine_index;
  int dest_task_index;
  int tct_increment;
};
