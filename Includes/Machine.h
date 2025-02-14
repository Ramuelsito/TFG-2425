/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Machine class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <iostream>
#include <vector>
#include "Tasks.h"

/**
 * @class Machine
 * @brief Class that represents a machine
 */
class Machine {
 public:
  Machine(int id, std::vector<Task> tasks_assigned, int tc_time = 0) :  id_(id), 
  tasks_assigned_(tasks_assigned) {}
  Machine(int id) : id_(id) { tasks_assigned_.clear(); }
  ~Machine() = default;
  
  int getId() const { return id_; }
  Task GetLastTask() const { return tasks_assigned_.back(); }
  int GetLastTaskTime() const { return last_task_time_; }
  std::vector<Task> getTasksAssigned() const { return tasks_assigned_; }

  int EmulateInsertion(const Task& task, int task_position) const;
  int EmulateRemoval(const int& task_index) const;
  void InsertTask(const Task& task, int task_position, int tct_increment = 0);
  void RemoveTask(const int& task_index, int tct_decrement);
  int RecalculateTotalCompletionTime();

  friend std::ostream& operator<<(std::ostream& os, const Machine& machine) {
    os << "Machine " << machine.id_ << ":\n";
    if (machine.tasks_assigned_.size() != 0) {
      os << "Tasks assigned: ";
      for (int i = 0; i < machine.tasks_assigned_.size(); i++) {
        os << machine.tasks_assigned_[i] << " ";
      }
      os << "\n";
    }
    return os;
  }
 private:
  int id_;
  int last_task_time_;
  std::vector<Task> tasks_assigned_;
};