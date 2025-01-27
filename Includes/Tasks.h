/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * @brief Task class declaration
 * @author alu0101471136@ull.edu.es (Raúl Álvarez)
 * @since Mar 27 2024  
 * @see {https://github.com/alu0101471136}
 */

#pragma once
#include <iostream>

/**
 * @class Task
 * @brief Class that represents the Task
 */
class Task {
 public: 
  Task() = default;
  Task(int id, int time) : id_{id}, time_{time} {}
  ~Task() = default;

  int GetId() const { return id_; }
  int GetTime() const { return time_; }
  void SetId(int id) { id_ = id; }
  void SetTime(int time) { time_ = time; }

  bool operator<(const Task& task) const { return time_ < task.time_; }
  bool operator==(const Task& task) const { return id_ == task.id_; }
  bool operator!=(const Task& task) const { return id_ != task.id_; }
  friend std::ostream& operator<<(std::ostream& os, const Task& task) {
    os << "Task " << task.id_ << ": [" << task.time_ << "]";
    return os;
  }
 private:
  int id_;
  int time_;
};