

#include "../Includes/Machine.h"


void Machine::ReInsertTask(const int& task_index, const int& new_task_position, const std::vector<std::vector<int>>& setup_times) {
  Task task = tasks_assigned_[task_index]; // Task to reinsert
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
  tasks_assigned_.insert(tasks_assigned_.begin() + new_task_position, task);
  RecalculateTotalCompletionTime(setup_times);
}

void Machine::SwapTasks(const int& task_index, const int& other_task_index, const std::vector<std::vector<int>>& setup_times) {
  Task first_task = tasks_assigned_[task_index]; // Task to reinsert
  Task second_task = tasks_assigned_[other_task_index];
  tasks_assigned_[task_index] = second_task;
  tasks_assigned_[other_task_index] = first_task;
  RecalculateTotalCompletionTime(setup_times);
}

void Machine::RemoveTask(const int& task_index, int tct_decrement) {
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
  tc_time_ -= tct_decrement;
}

void Machine::InsertTask(const Task& task, int task_position, int tct_increment) { 
  tasks_assigned_.insert(tasks_assigned_.begin() + task_position, task);
  tc_time_ += tct_increment;
}

void Machine::RecalculateTotalCompletionTime(const std::vector<std::vector<int>>& setup_times) {
  tc_time_ = 0;
  int unitary_time = 0;
  int tij = 0;
  for (int i = 0; i < tasks_assigned_.size(); i++) {
    if (i == 0) {
      tij = tasks_assigned_[i].GetTime() + setup_times[0][tasks_assigned_[i].GetId() + 1];
    } else {
      tij = tasks_assigned_[i].GetTime() + setup_times[tasks_assigned_[i - 1].GetId() + 1][tasks_assigned_[i].GetId() + 1];
    }
    tc_time_ += unitary_time + tij;
    unitary_time += tij;
  }
}