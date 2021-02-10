//  This file is part of Evolutionary Algorithm
//  Copyright (C) Anya Vostinar, 2021.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/base/vector.hpp"
#include "emp/config/command_line.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/config/ArgManager.hpp"

#include "World.h"

// This is the main function for the NATIVE version of this project.
EMP_BUILD_CONFIG(MyConfigType,
    GROUP(DEFAULT, "Default settings for NK model"),
    VALUE(SEED, int, 10, "What value should the random seed be?"), 
    VALUE(START_PROB, double, 0.5, "What cooperation probability value should the starting organism have?"),
    VALUE(FILE_PATH, std::string, "", "Output file path"),
    VALUE(FILE_NAME, std::string, "_data.dat", "Root output file name")
)


int main(int argc, char* argv[])
{
  MyConfigType config;
  bool success = config.Read("MySettings.cfg");
  if(!success) config.Write("MySettings.cfg");


  auto args = emp::cl::ArgManager(argc, argv);
  if (args.ProcessConfigOptions(config, std::cout, "SymSettings.cfg") == false) {
  std::cerr << "There was a problem in processing the options file." << std::endl;
  exit(1);
}
if (args.TestUnknown() == false) exit(0);  // If there are leftover args, throw an error.

  emp::Random random(config.SEED());
  OrgWorld world(random);

  world.SetupOrgFile(config.FILE_PATH()+"Org_Vals"+std::to_string(config.SEED())+config.FILE_NAME());
  

  emp::Ptr<Organism> new_org = new Organism(&random, 0.5);
  world.Inject(*new_org);
  world.Resize(100,100);
  
  for(int i=0; i<1000; i++) {
    //std::cout<< "Update: " << i << std::endl;
    //std::cout << "Population: " << world.GetNumOrgs() << std::endl;
    world.Update();
  }


}
