#ifndef SWARM_SEARCH_H
#define SWARM_SEARCH_H

#include <cstdlib>
#include "Optimization/Optimization_Strategy.h"
#include "Optimization/Bees_Algorithm.h"
#include "Process/Process.h"
#include "Process/Process_Manager.h"

using namespace std;

enum SEARCH_TYPE
{
    BEES, ABC, ACO, ANT, PSO, MULTI_SWARM, GRAVITATIONAL, GP
};

class Swarm_Search
{
  public:

      Swarm_Search (SEARCH_TYPE enum_strategy = BEES)
                   :p_strategy(nullptr), enum_strategy(enum_strategy)
      {
          setStrategy(enum_strategy);
      };
      ~Swarm_Search() {};

      void setStrategy(SEARCH_TYPE strat)
      {
          switch(strat)
          {
            case BEES:
                p_strategy.reset(new Bees_Algorithm());
                break;
            case ACO:
                //TODO
                break;
            case ABC:
                //TODO
                break;
            case PSO:
                //TODO
                break;
            case MULTI_SWARM:
                //TODO
                break;
            case GRAVITATIONAL:
                //TODO
                break;
            case GP:
                //TODO
                break;
            default:
                //nothing
                break;
          }

      };

      void printStrategy()
      {
          cout<<"Search Strategy: "<< Strategy_Type_Str[enum_strategy]<<endl;;
      };

      void initialise()
      {
          p_strategy->initialise();
      };

      void search()
      {
          p_strategy->search();
      };

  private:
      unique_ptr<Optimization_Strategy> p_strategy;
      double common_due_date;

      SEARCH_TYPE enum_strategy;
      const string Strategy_Type_Str[5] = {"Bees Algorithm",
                                           "Ant Colony Optimization",
                                           "Particle Swarm Optimization",
                                           "Multi-Swarm Optimization",
                                           "Gravitational Search Algorithm"};



};

#endif // SWARM_SEARCH_H
