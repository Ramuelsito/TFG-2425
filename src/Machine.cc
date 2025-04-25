

#include "../Includes/Machine.h"
#include "../Includes/Problem.h"

/**
 * @brief Given a task and a position, emulates the insertion of the task in the machine
 * @param task The task to insert
 * @param q The position where the task is going to be inserted
 * @return The increment of the total completion time
 */
int Machine::EmulateInsertion(const Task& task, int q) const {
  int tasks_assigned_size = tasks_assigned_.size();
  std::shared_ptr<Problem> problem = Problem::getInstance();
  int tct_increment = 0;
  if (tasks_assigned_size == 0) { 
    tct_increment = task.GetTime() + problem->CalculateSij(0, task.GetId() + 1);
    return tct_increment;
  } 
  if (q == 0) {
    int new_t0i = task.GetTime() + problem->CalculateSij(0, task.GetId() + 1);
    int old_t01 = tasks_assigned_[0].GetTime() + problem->CalculateSij(0, tasks_assigned_[0].GetId() + 1);
    int new_tij = tasks_assigned_[0].GetTime() + problem->CalculateSij(task.GetId() + 1, tasks_assigned_[0].GetId() + 1);
    tct_increment = (tasks_assigned_size + 1) * new_t0i + tasks_assigned_size * (new_tij - old_t01);
  } else if((1 <= q) && (q < tasks_assigned_size)) {
    int other_t01 = tasks_assigned_[0].GetTime() + problem->CalculateSij(0, tasks_assigned_[0].GetId() + 1);
    tct_increment = other_t01;
    for (int l = 1; l <= q - 1; l++) {
      int other_tij = tasks_assigned_[l].GetTime() + problem->CalculateSij(tasks_assigned_[l - 1].GetId() + 1, tasks_assigned_[l].GetId() + 1);
      tct_increment += other_tij;
    }
    int new_tij = task.GetTime() + problem->CalculateSij(tasks_assigned_[q - 1].GetId() + 1, task.GetId() + 1);
    int new_tiq = tasks_assigned_[q].GetTime() + problem->CalculateSij(task.GetId() + 1, tasks_assigned_[q].GetId() + 1);
    int aux_tij = tasks_assigned_[q].GetTime() + problem->CalculateSij(tasks_assigned_[q - 1].GetId() + 1, tasks_assigned_[q].GetId() + 1);
    tct_increment += (tasks_assigned_size - (q + 1) + 2) * new_tij + (tasks_assigned_size - (q + 1) + 1) * (new_tiq - aux_tij);
  } else if (q == tasks_assigned_size) { // Tenemos que tener cuidado porque si q = tasks_assigned_size, tasks_assigned_[q] no existe
    int other_t01 = tasks_assigned_[0].GetTime() + problem->CalculateSij(0, tasks_assigned_[0].GetId() + 1);
    tct_increment = other_t01;
    for (int l = 1; l < tasks_assigned_size; l++) {
      int other_tij = tasks_assigned_[l].GetTime() + problem->CalculateSij(tasks_assigned_[l - 1].GetId() + 1, tasks_assigned_[l].GetId() + 1);
      tct_increment += other_tij;
    }
    int new_tij = task.GetTime() + problem->CalculateSij(tasks_assigned_[tasks_assigned_size - 1].GetId() + 1, task.GetId() + 1);
    tct_increment += new_tij;
  }
  return tct_increment;
}

/**
 * @brief Given a task index, emulates the removal of the task in the machine
 * @param task_index The index of the task to remove
 * @return The decrement of the total completion time
 */
int Machine::EmulateRemoval(const int& i) const {
  int tasks_assigned_size = tasks_assigned_.size();
  std::shared_ptr<Problem> problem = Problem::getInstance();
  int tct_decrement = 0;
  if (i == 0) {
    int new_t0j = tasks_assigned_[i + 1].GetTime() + problem->CalculateSij(0, tasks_assigned_[i + 1].GetId() + 1);
    int old_tij = tasks_assigned_[i + 1].GetTime() + problem->CalculateSij(tasks_assigned_[i].GetId() + 1, tasks_assigned_[i + 1].GetId() + 1);
    int t0i = tasks_assigned_[i].GetTime() + problem->CalculateSij(0, tasks_assigned_[i].GetId() + 1);
    tct_decrement = (tasks_assigned_size - 1) * (new_t0j - old_tij) - tasks_assigned_size * t0i;
  } else if((1 <= i) && (i < tasks_assigned_size - 1)) {
    int new_tij = tasks_assigned_[i + 1].GetTime() + problem->CalculateSij(tasks_assigned_[i - 1].GetId() + 1, tasks_assigned_[i + 1].GetId() + 1);
    int old_tij = tasks_assigned_[i + 1].GetTime() + problem->CalculateSij(tasks_assigned_[i].GetId() + 1, tasks_assigned_[i + 1].GetId() + 1);
    int tij = tasks_assigned_[i].GetTime() + problem->CalculateSij(tasks_assigned_[i - 1].GetId() + 1, tasks_assigned_[i].GetId() + 1);
    int current_til = tasks_assigned_[0].GetTime() + problem->CalculateSij(0, tasks_assigned_[0].GetId() + 1);
    for (int l = 1; l <= i - 1; l++) {
      int til = tasks_assigned_[l].GetTime() + problem->CalculateSij(tasks_assigned_[l - 1].GetId() + 1, tasks_assigned_[l].GetId() + 1);
      current_til += til;
    }
    tct_decrement = (tasks_assigned_size - (i + 1)) * (new_tij - old_tij) - (tasks_assigned_size - (i + 1) + 1) * tij - current_til;
  } else if (i == tasks_assigned_size - 1) {
    int other_t01 = tasks_assigned_[0].GetTime() + problem->CalculateSij(0, tasks_assigned_[0].GetId() + 1);
    tct_decrement = other_t01;
    for (int l = 1; l < tasks_assigned_size; l++) {
      int til = tasks_assigned_[l].GetTime() + problem->CalculateSij(tasks_assigned_[l - 1].GetId() + 1, tasks_assigned_[l].GetId() + 1);
      tct_decrement += til;
    }
    tct_decrement *= -1;
  }
  return tct_decrement;
}

void Machine::RemoveTask(const int& task_index, int tct_decrement) {
  tasks_assigned_.erase(tasks_assigned_.begin() + task_index);
}

void Machine::InsertTask(const Task& task, int task_position, int tct_increment) { 
  tasks_assigned_.insert(tasks_assigned_.begin() + task_position, task);
  // std::cout << "TCT: " << tc_time_ << " + " << tct_increment << " = " << tc_time_ + tct_increment << std::endl;
  last_task_time_ = tct_increment;
}

/**
 * @brief Given two indexes, exchanges the tasks in the machine
 * @param i The first task index
 * @param j The second task index
 * @return void
 */
void Machine::ExchangeTasks(const int& i, const int& j) {
  Task aux = tasks_assigned_[i];
  tasks_assigned_[i] = tasks_assigned_[j];
  tasks_assigned_[j] = aux;
}

int Machine::RecalculateTotalCompletionTime() {
  std::vector<std::vector<int>> setup_times = Problem::getInstance()->getSetupTimes();
  int k = tasks_assigned_.size();
  int tc_time = k * (tasks_assigned_[0].GetTime() + setup_times[0][tasks_assigned_[0].GetId() + 1]); 
  for (int i = 1; i < tasks_assigned_.size(); i++) {
    tc_time += (k - i) * (tasks_assigned_[i].GetTime() + setup_times[tasks_assigned_[i - 1].GetId() + 1][tasks_assigned_[i].GetId() + 1]);
  }
  return tc_time;
}