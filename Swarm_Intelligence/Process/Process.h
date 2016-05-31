#ifndef PROCESS_H
#define PROCESS_H

#include <thread>

using namespace std;

/**
 * -Class simulating a process to be scheduled.
 * -Single machine scheduling is being simulated, so all processes will run on
 *  a single thread, which will sleep to simulate execution time of each
 *  process.
 * -The process need not actually be "executed", the focus of this class is more
 *  as a container of related data for individual processes
 * -This class is an approximation of the general case and is not problem specific,
 *  therefore certain members are superfluous.
 *
 **/

typedef std::chrono::duration<double, milli> time_duration;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_stamp;

class Process
{
  public:
      Process() {};
      Process(double processing_time,
              double earliness_penalty,
              double tardiness_penalty)
             :m_processing_time(processing_time),
              m_earliness_penalty(earliness_penalty),
              m_tardiness_penalty(tardiness_penalty)

      {};
      /*Process(double processing_time,
              double earliness_penalty,
              double tardiness_penalty,
              double start_time,
              double completion_time,
              double m_due_date)
             :m_processing_time(processing_time),
              m_earliness_penalty(earliness_penalty),
              m_tardiness_penalty(tardiness_penalty).

      {};*/

      //copy constructor
      /*Process(const Process& process)
             :m_processing_time(process.m_processing_time), m_earliness_penalty(process.m_earliness_penalty), m_tardiness_penalty(process.m_tardiness_penalty)
      {};

      //move constructor
      Process(Process&& process)
             :m_processing_time(process.m_processing_time), m_earliness_penalty(process.m_earliness_penalty), m_tardiness_penalty(process.m_tardiness_penalty)
      {};*/

      //destructor
     ~Process() {};

      /** Utility */
      void execute()
      {
          this_thread::sleep_for( time_duration(m_processing_time));
      };

      //fitness is always positive (both earliness and tardiness and constants are positive)
      //this fitness is problem specific; it would be better to pass a function pointer or use a lambda
      //expression for a more generic approach
      double calculateFitness()
      {
         m_earliness = 0.0; //due_date - completion_time
         m_tardiness = 0.0; //completion_time - due_date

         if(m_completion_time < m_due_date)
             m_earliness = m_due_date - m_completion_time;
         else
            m_tardiness = m_completion_time - m_due_date;

         m_fitness = ((m_earliness_penalty * m_earliness) + (m_tardiness_penalty * m_tardiness));

 //testing
/*cout<<"time: "<<m_processing_time
     <<" e pen: "<<m_earliness_penalty
     <<" t pen: "<<m_tardiness_penalty
     <<" e : "<<m_earliness
     <<" t : "<<m_tardiness
     <<" fitness: "<<m_fitness<<endl;//*/

         return m_fitness;
      };

      /** Mutators */
      void setID(int process_ID)                     { m_process_ID = process_ID; };
      void setProcessingTime(double processing_time) { m_processing_time = processing_time; };
      void setEarliness(double earliness)            { m_earliness = earliness; };
      void setTardiness(double tardiness)            { m_tardiness = tardiness; };

      void setStartTime(double start_time)             { m_start_time = start_time; };
      void setCompletionTime(double completion_time)   { m_completion_time = completion_time; };

      void setDueDate(double due_date)                 { m_due_date = due_date; };

      void setEarlinessPenalty(double earliness_penalty)    { m_earliness_penalty = earliness_penalty; };
      void setTardinessPenalty(double tardiness_penalty)    { m_tardiness_penalty = tardiness_penalty; };

      /** Accessors */
      const int getID()                const{ return m_process_ID; };
      const double getProcessingTime() const{ return m_processing_time; };
      const double getEarliness()      const{ return m_earliness; };
      const double getTardiness()      const{ return m_tardiness; };

      const double getStartTime()      const { return m_start_time; };
      const double getCompletionTime() const { return m_completion_time; };
      const double getDueDate()        const { return m_due_date; };

      double getEarlinessPenalty() const{ return m_earliness_penalty; };
      double getTardinessPenalty() const{ return m_tardiness_penalty; };

      double getFitness() const { return m_fitness; };

      /** Operator Overload */
      //copy assignment operator
      /*Process operator=(const Process& other_process)
      {
          return Process(other_process.m_processing_time,
                         other_process.m_earliness_penalty,
                         other_process.m_tardiness_penalty);
      };

      //move assignment operator
      Process operator=(Process&& other_process)
      {
          return Process(other_process.m_processing_time, other_process.m_earliness_penalty, other_process.m_tardiness_penalty);
      };*/

  private:
      int m_process_ID = 0; //0 is meaningless, any process with ID 0 was incorrectly initialized
      double m_processing_time; //time taken to complete task
      double m_earliness = 0.0; //due_date - completion_time
      double m_tardiness = 0.0; //completion_time - due_date

      double m_start_time = 0.0;
      double m_completion_time = 0.0;

      double m_due_date;

      double m_earliness_penalty;
      double m_tardiness_penalty;

      double m_fitness;
};

#endif // PROCESS_H
