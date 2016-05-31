#ifndef BEES_ALGORITHM_H
#define BEES_ALGORITHM_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <math.h>
#include <random>
#include <time.h>
#include <thread>
#include <map>
#include <algorithm>

#include "Optimization_Strategy.h"
#include "Random/randutils.hpp"

using namespace std;

/**
 * • SCHEDULING SPECIFICATIONS
 * -All jobs will be processed without interruption on a single machine.
 * -All jobs are available from time zero.
 * -The goal is to minimize a jobs earliness and tardiness (minimize the sum of the
 *  repspective penalties).
 * -The objective function makes use of tardiness and earliness penality
 *  constants which it aims to mimimize.
 *
 * •OPTIMAL SCHEDULING PROPERTIES
 * 1. There are no idle times between consecutive jobs.
 * 2. An optimal schedule has the "V-Shape" property.
 * 3. There exists an optimal schedule where either the processing time of the
 *    first job starts at zero or one job finished at the due date.
 *
 * •OPTIMAL SCHEDULING CASES
 * 1. The first job starts at time 0 and the last job finishes at due date.
 * 2. The first job starts at time 0 and the last job finishes before due date,
 *    therefore a straddling job exists.
 * 3. The first job does not necessarily start at time 0.
 *
 *
 * • ALGORITHM SPECIFICATIONS
 * -Number of Bees: if    the number of jobs(n) < 100, b = 2n
 *                  else  b = 400
 *
 * -Bees are represented as structs storing ID, type, fitness, and foraging location of the bee
 * -The hive is represented by a vector holding bee objects
 * -Bees of different types are stored in seperate vectors as pointers to bee objects in the hive.
 *
 * -Stochastic Optimization Algorithms
 * -Exploitative search(local)
 * -Performs neighbourhood combined with global search
 * -Can be used for combinatorial and contiunuous optimization problems
 * -Breaks SRP: this class contains functions for loading data sets and storing them in
 *              a manner which provides logical storage access abstracted from reality.
 *        -FIX: since there are many optimization strategies, create a class to manage
 *              different loading/storage types which are linked to "Optimization_Strategy"
 *              and dynamically bound to a secific implementation.
 *
 * • ALGORITHM
 *  1. Place scout bees randomly (each scout visits a single location)
 *    -Scout bees are randomly places in the search space
 *    -The search space is defined as the collection of all generated soloutions
 *    -Scout bees will be sent out on seperate threads, within the thread, a position will
 *     be randomly chosen and the bees operations will be performed (including fitness)
 *    -?????How many initial scout bees? The entire population????
 *
 *  2. Calculate fitness of sites visited by scouts
 *    -The solution visited by each scout is put through the fitness function
 *    -Once all threads are finished, ranks the fitnesses
 *
 *  >WHILE(STOPPING CONDITION) //stopping condition is implementation dependent
 *  3. Choose bees with highest fitness and choose their sites for a neighbourhood search
 *    -The fitness of the scout bees' solutions is ranked upon their return.
 *    -These highest fitness sites are chosen to be above some threshold
 *     or a fixed number of sites is chosen (??unsure).
 *    -Article states that high fitness sites are chosen to be above the maximum of the
 *     fitness function. In this case the max is one where the earliness and tardiness
 *     of a particulat process are are at minimum.
 *
 *  4. Recruit bees to send to selected sites
 *    -The recruited bees do not visit the same solution as their leader scout bee, they
 *     visit random sites within the neighbourhood hyperspace.
 *    -The general point is that sites with higher fitness will have a higher concentration
 *     of their neighbourhood "flower patch" searched
 *    -The patch size should be defined, and recruited bees should search a point on
 *     either side of their scouts position (our search space is linear and combinatorial)
 *     within the neighbourhood boundaries.
 *
 *  5. Select the fittest bee from each patch searched
 *    -Once all recruited bee threads are complete, rank the fitness of each patch,
 *     including the scout bee which remains unaltered
 *
 *  6. Assign remaining bees to search randomly and evaluate fitness (search for new areas)
 *    -These will be the new scout bees.
 *  >END WHILE
 *
 *  -The goal of an iteration of the search is to obtain two sets of results, from the local and global
 *   searches. The best of these 2 sets of bees will be chosen as the new population in the second cycle
 *   etc..
 *
 *
 **/

class Bees_Algorithm : public Optimization_Strategy
{
  ///Define bees
  private:
      enum BEE_TYPE
      {
          SCOUT, RECRUITED, REMAINING
      };

      struct Bee
      {
          Bee(int bee_ID) : bee_ID(bee_ID){};

          //bee identifiers
          int bee_ID;
          BEE_TYPE type = SCOUT;

          double fitness = 0;
          int location; //process id visited by the bee

          Process* process;

          /** Operator overload*/
          bool operator <(const Bee& bee)
          {
              if(this->fitness < bee.fitness)
                  return true;
              else
                  return false;
          };

          bool operator >(const Bee& bee)
          {
              if(this->fitness > bee.fitness)
                  return true;
              else
                  return false;
          };
      };

  public:
      Bees_Algorithm() {};
      /*Bees_Algorithm(int num_scoutBees, int size_patches)
                    :num_scoutBees(num_scoutBees), total_bees(num_scoutBees),
                     size_patches(size_patches)
      {};*/ //these should be defined by the algorithm itself, no args should be passed
      //perhaps initialise with a command object
      virtual ~Bees_Algorithm(){};

      /** Utility */
      virtual void initialise();
      void initialiseBees();

      virtual void search();
      //virtual void loadData(const string filename);

      virtual void evaluateSolutionFitness();
      void evaluateBeeFitness(vector<Bee*> bees);

      /** Accessors */
      int getNumBees() { return total_bees; };

      /** Mutators */
      void setTotalBees(int total_bees) { this->total_bees = total_bees; };
      void setNumIterations(int num_iterations) { this->num_iterations = num_iterations; };

  private:
      int num_iterations = 1;

      //variables
      int total_bees = 400;        // (p) population - also initial no. scouts
      int num_scoutBees;     // (b) number of scouts on successive runs
      int num_recruitedBees = 50; // (nep) to be sent to the best sites
      int num_remainingBees;  // (nsp) bees to be sent to the remaining selected sites

      const int num_selectedSites = 200; // (m) number of sites selected out of sites visited (b)
      //const int num_bestSites = 100;      // (e) number of best sites out of selected sites

      const int size_patches = 6; // (ngh) initial size of the patches (site, neighbourhood, stopping criterion)

      vector<Bee> hive;

      //points to Bee objects in hive and classifies them(prevents the need to iterate through entire hive and check bee type)
      vector<Bee*> scout_bees;
      //vector<Bee*> recruited_bees;
      vector<Bee*> remaining_bees;

      //map of location(solution space index) of patches, constaining vector of bees at that patch
      map<int, vector<Bee*>> patch_locations;
      //vector<vector<Bee*>> Patches;

      //typedef Process_Manager Patch;
      //vector<Patch> Patches; //contains numb_patches patches
};

#endif // BEES_ALGORITHM
