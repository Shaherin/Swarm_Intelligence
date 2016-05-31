#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include<iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include "Process.h"

using namespace std;

/**
 * -Manages a group of processes
 * -How this class is used is determined by a particular optimization algorithm
 *
 **/
class Process_Manager
{
  public:
      Process_Manager() {};

      //move constructor
      /*Process_Manager(Process_Manager&& manager)
          :m_num_processes(manager.m_num_processes), m_processes(manager.m_processes),
           m_available_IDs(manager.m_available_IDs), m_restrictive_factor(manager.m_restrictive_factor)
      {};//*/

      //copy constructor
      /*Process_Manager(const Process_Manager& manager)
          :m_num_processes(manager.m_num_processes), m_processes(manager.m_processes),
           m_available_IDs(manager.m_available_IDs), m_restrictive_factor(manager.m_restrictive_factor)
      {};//*/

     ~Process_Manager() {};

      /** Utility */
      void addProcess(Process process);
      void addProcesses(vector<Process> processes);
      void removeProcess(int process_ID);
      void shuffleProcesses();
      void clearProcesses();

      //initialises start and completion time of processes once all processes are added
      void initialiseProcesses();
      //d = round [SUM_P * h] - set this value within contained processes
      void calcCommonDueDate(double restrictive_factor = 0.8);
      //calculates fitness of all processes contained in this manager
      double calculateFitness();

      /** Mutators */
      void setRestrictiveFactor(double restrictive_factor) { m_restrictive_factor = restrictive_factor; };

      /** Accessors */
      //get members
      const vector<Process>& getProcesses()  const{ return m_processes; };
      //const double& getFitness()           const  { return m_fitness; };
      //const double& getCommonDueDate()     const  { return m_common_due_date; };
      const double getRestrictiveFactor()    const{ return m_restrictive_factor; };
      const double getDueDate()              const{ return m_common_due_date; };
      const double getFitness()              const{ return m_total_fitness; };

      //get other
      const int getNumProcesses()               const{ return m_num_processes - m_available_IDs.size(); };
      const Process& getProcess(int process_ID) const{ return m_processes[process_ID]; };

      /** Operator Overload*/
      //Process_Manager is essentially a wrapper for a map
      const Process& operator[](int index)
      {
          return m_processes[index];
      };

      /*Process_Manager operator=(const Process_Manager& other_process_manager)
      {
          return Process_Manager(other_process_manager);
      };//*/

  private:
      int m_num_processes = 0;
      vector<Process> m_processes;

      //if a process is removed, its ID is stored to be reassigned with higher priority
      vector<int> m_available_IDs;

      double m_total_fitness; //f(S) fitness of all combined processes
      double m_common_due_date; // (d)
      double m_restrictive_factor = 0.4; // (h) used in estimating common due date

};


#endif // PROCESS_MANAGER_H
