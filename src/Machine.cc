

#include "../Includes/Machine.h"
#include "../Includes/Problem.h"


void Machine::ReInsertTask(const int& task_index, const int& new_task_position, const std::vector<std::vector<int>>& setup_times) {
  Task task = tasks_assigned_[task_index]; // Task to reinsert
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
  tasks_assigned_.insert(tasks_assigned_.begin() + new_task_position, task);
  RecalculateTotalCompletionTime();
}

void Machine::SwapTasks(const int& task_index, const int& other_task_index, const std::vector<std::vector<int>>& setup_times) {
  Task first_task = tasks_assigned_[task_index]; // Task to reinsert
  Task second_task = tasks_assigned_[other_task_index];
  tasks_assigned_[task_index] = second_task;
  tasks_assigned_[other_task_index] = first_task;
  RecalculateTotalCompletionTime();
}

void Machine::RemoveTask(const int& task_index, int tct_decrement) {
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
  tc_time_ -= tct_decrement;
}

void Machine::InsertTask(const Task& task, int task_position, int tct_increment) { 
  tasks_assigned_.insert(tasks_assigned_.begin() + task_position, task);
  // std::cout << "TCT: " << tc_time_ << " + " << tct_increment << " = " << tc_time_ + tct_increment << std::endl;
  tc_time_ += tct_increment;
  last_task_time_ = tct_increment;
}

void Machine::RecalculateTotalCompletionTime() {
  std::vector<std::vector<int>> setup_times = Problem::getInstance().getSetupTimes();
  int k = tasks_assigned_.size();
  tc_time_ = k * (tasks_assigned_[0].GetTime() + setup_times[0][tasks_assigned_[0].GetId() + 1]); 
  for (int i = 1; i < tasks_assigned_.size(); i++) {
    tc_time_ += (k - i) * (tasks_assigned_[i].GetTime() + setup_times[tasks_assigned_[i - 1].GetId() + 1][tasks_assigned_[i].GetId() + 1]);
  }
}