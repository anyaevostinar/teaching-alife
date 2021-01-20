#pragma once

#include <iostream>

#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/data/DataFile.hpp"

#include "fitness.h"
#include "selection.h"

void evolve() {
  const size_t population_size = 50;
  const size_t gens = 100;
  size_t curr_gen = 0;

  // make random engine
  emp::Random rand(-1);

  // vector to store our population,
  // fill it with randomized genomes between 0 and 1
  emp::vector<double> population;
  population = emp::RandomDoubleVector(
    rand,
    population_size,
    0.0,
    1.0
  );

auto datafile = emp::MakeContainerDataFile(
    std::function<emp::vector<double>()>{
      [&population](){ return population; }
    },
    "evo-algo.csv"
  );

    datafile.AddVar(
    curr_gen,
    "generation",
    "Current Generation"
  );

  datafile.AddContainerFun(
    std::function<double(double)>{[](double x){
      return x;
    }},
    "genome",
    "Genome's content"
  );

  datafile.AddContainerFun(
    std::function<double(double)>{[](double x){
      return calcFitness(x);
    }},
    "fitness",
    "Genome's Fitness"
  );

  datafile.PrintHeaderKeys();
  datafile.Update();

  while (++curr_gen < gens) {
    emp::vector<double> next_population;
    // select individuals for next generation
    for (size_t i = 0; i < population_size; ++i) {
      double winner = doTournament(
        population,
        rand,
        3
      );
      next_population.push_back(winner);
    }

        // do mutation
    for (double& ind : next_population) {
      if (rand.P(0.25)) {
        ind += rand.GetDouble(-1.0, 1.0);
      }
    }

    population = next_population;

    datafile.Update();
  }

}