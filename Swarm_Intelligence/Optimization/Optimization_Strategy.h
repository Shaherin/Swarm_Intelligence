#ifndef OPTIMIZATION_STRATEGY_H
#define OPTIMIZATION_STRATEGY_H

#include <cstdlib>

#include "Process.h"
#include "Process_Manager.h"

using namespace std;

/**
 * - Virtual base class of the strategy design pattern
 *
 **/
class Optimization_Strategy
{
  public:
      Optimization_Strategy(){};
      virtual ~Optimization_Strategy() {};

      virtual void initialise() = 0;
      virtual void search() = 0;

      virtual void loadData(const string filename) = 0;

      //evaluates the fitness of the eventual solution found by the strategy
      virtual void evaluateSolutionFitness() = 0;

  protected:
      Process_Manager Processes;

      //data filepaths
      const string sch10   = "Data Files/sch10.txt";
      const string sch20   = "Data Files/sch20.txt";
      const string sch50   = "Data Files/sch50.txt";
      const string sch100  = "Data Files/sch100.txt";
      const string sch200  = "Data Files/sch200.txt";
      const string sch500  = "Data Files/sch500.txt";
      const string sch1000 = "Data Files/sch1000.txt";


};

#endif // OPTIMIZATION_STRATEGY_H
