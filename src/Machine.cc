

#include "../Includes/Machine.h"


void Machine::ReInsertTask(const int& task_index, const int& new_task_position, const std::vector<std::vector<int>>& setup_times) {
  Task task = tasks_assigned_[task_index]; // Task to reinsert
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
  tasks_assigned_.insert(tasks_assigned_.begin() + new_task_position, task);
  RecalculateTotalCompletionTime(setup_times);
}

void Machine::SwapTasks(const int& task_index, const int& other_task_index, const std::vector<std::vector<int>>& setup_times) {
  // Task first_task = tasks_assigned_[task_index]; // Task to reinsert
  // Task second_task = tasks_assigned_[other_task_index];
  // tasks_assigned_[task_index] = second_task;
  // tasks_assigned_[other_task_index] = first_task;
  // int first_TCT = total_completion_times_[task_index];
  // int second_TCT = total_completion_times_[other_task_index];
  // int first_tij = first_task.GetTime() + setup_times[task_index][task_index + 1];
  // int second_tij = second_task.GetTime() + setup_times[other_task_index][other_task_index + 1];
  // total_completion_times_[task_index] = second_TCT - first_tij + second_tij;
  // total_completion_times_[other_task_index] = first_TCT - second_tij + first_tij;
  // tc_time_ -= first_TCT + second_TCT;
  // tc_time_ += total_completion_times_[task_index] + total_completion_times_[other_task_index];

  Task first_task = tasks_assigned_[task_index]; // Task to reinsert
  Task second_task = tasks_assigned_[other_task_index];
  tasks_assigned_[task_index] = second_task;
  tasks_assigned_[other_task_index] = first_task;
  RecalculateTotalCompletionTime(setup_times);
}

void Machine::RemoveTask(const int& task_index) {
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
  total_completion_times_.erase(total_completion_times_.begin() + task_index);
}

void Machine::InsertTask(const Task& task, const int& task_index) {
  tasks_assigned_.insert(tasks_assigned_.begin() + task_index, task);
  total_completion_times_.insert(total_completion_times_.begin() + task_index, 0);
}

void Machine::AddTask(Task task, int task_time) { 
  
  tc_time_ += unitary_time_ + task_time;
  unitary_time_ += task_time;
  tasks_assigned_.push_back(task); 
  total_completion_times_.push_back(unitary_time_);
}

bool Machine::CheckTotalCompletionTime(int time_to_add, int min_time_got) {
  int tc_time_copy = tc_time_ + unitary_time_ + time_to_add;
  if (tc_time_copy < min_time_got) {
    return true;
  }
  return false;
}

void Machine::RecalculateTotalCompletionTime(const std::vector<std::vector<int>>& setup_times) {
  tc_time_ = 0;
  unitary_time_ = 0;
  int tij = 0;
  for (int i = 0; i < tasks_assigned_.size(); i++) {
    if (i == 0) {
      tij = tasks_assigned_[i].GetTime() + setup_times[0][tasks_assigned_[i].GetId() + 1];
    } else {
      tij = tasks_assigned_[i].GetTime() + setup_times[tasks_assigned_[i - 1].GetId() + 1][tasks_assigned_[i].GetId() + 1];
    }
    tc_time_ += unitary_time_ + tij;
    unitary_time_ += tij;
    total_completion_times_[i] = unitary_time_;
  }
}