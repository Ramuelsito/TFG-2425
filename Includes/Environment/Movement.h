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

struct Movement
{
  int orig_machine_index;
  int dest_machine_index;
  int orig_task_index;
  int dest_task_index;
  int new_tct;
};
