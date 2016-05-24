#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

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
     ~Process_Manager() {};

      /** Utility */
      void addProcess(Process process);
      void addProcesses(vector<Process> processes);
      void removeProcess(int process_ID);
      void clearProcesses();

      //d = round [SUM_P * h]
      double calcCommonDueDate(vector<Process*> processes);
      //calculates fitness of a group of processes
      double calculateFitness(vector<Process*> processes);

      /** Mutators */
      void setRestrictiveFactor(double restrictive_factor) { m_restrictive_factor = restrictive_factor; };

      /** Accessors */
      //get members
      const map<int, Process>& getProcesses() { return m_processes; };
      //const double& getFitness()              { return m_fitness; };
      //const double& getCommonDueDate()        { return m_common_due_date; };
      const double& getRestrictiveFactor()    { return m_restrictive_factor; };

      //get other
      const int getNumProcesses()               { return m_num_processes - m_available_IDs.size(); };
      const Process& getProcess(int process_ID) { return m_processes[process_ID]; };

      /** Operator Overload*/
      //Process_Manager is essentially a wrapper for a map
      Process& operator[](int index)
      {
          return m_processes[index];
      };

  private:
      map<int, Process> m_processes; //process id, process
      int m_num_processes = 0;

      //if a process is removed, its ID is stored to be reassigned with higher priority
      vector<int> m_available_IDs;

      //double m_total_fitness; //f(S) fitness of all combined processes
      //double m_common_due_date; // (d)
      double m_restrictive_factor; // (h) used in estimating common due date

};


#endif // PROCESS_MANAGER_H
