// =====================================================================================
// 
//       Filename:  test.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 18:53:19
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "db/SednaDBInterface.class.h"
#include "simulation/StandardFSSimulation.class.h"
#include "process/AgentPopulation.class.h"
#include "rng/generator/UniformGenerator.class.h"
#include "rng/generator/inverse_cdf/ParetoGenerator.class.h"
#include "agent/ClassicAgentGenerator.class.h"
#include "callback_log/FileCallbackLog.class.h"
#include "boost/progress.hpp"

#include <exception>
#include <iostream>
#include <sstream>

using namespace GLOOPER_TEST;
using namespace RNG;
using namespace std;

int main(int argc, char** argv)
{
   try
   {
      if(argc < 3) throw "glooper: Must provide at least 2 command-line arguments";

      unsigned long simid = atol(argv[1]);

      if(simid==0) throw "glooper: A zero value for simulation_id has been provided or no conversion could be performed";

      FileCallbackLog F(argv[2],"glooper",TRACE);

      SednaDBInterface dbi("SimulationDB","Bender Rodriguez","bmsma_DTAH1","SimulationData");

      boost::shared_ptr<UniformGenerator> belief(new UniformGenerator);

      boost::shared_ptr<ParetoGenerator> wealth(new ParetoGenerator(10000,1.4));

      boost::shared_ptr<UniformGenerator> f_min(new UniformGenerator(0.05,0.1));

      boost::shared_ptr<UniformGenerator> p_min(new UniformGenerator(0.05,0.1));

      boost::shared_ptr<UniformGenerator> bas_max(new UniformGenerator(0.001,0.01));

      boost::shared_ptr<Market> mkt(new Market);

      boost::shared_ptr<ClassicAgentGenerator> ag(new ClassicAgentGenerator(belief,wealth,f_min,p_min,bas_max,mkt,20));

      boost::shared_ptr<UniformGenerator> info(new UniformGenerator);

      boost::shared_ptr<AgentPopulation> ap(new AgentPopulation(info,ag));

      ostringstream oss;

      boost::progress_timer* t = new boost::progress_timer(oss);

      StandardFSSimulation sim(ap,"full_cycle_test",dbi,simid,1,1,20);

      sim.simulate();

      delete t;

      LOG( INFO, boost::format("Simulation executed in %s\n") % oss.str() );
   }
   catch(const char* c)
   {
      cerr << "glooper: An exception has been caught: " << c << endl;

      return EXIT_FAILURE;
   }
   catch(const string& s)
   {
      cerr << "glooper: An exception has been caught: " << s << endl;

      return EXIT_FAILURE;
   }
   catch(const std::exception& e)
   {
      cerr << "glooper: An exception has been caught: " << e.what() << endl;

      return EXIT_FAILURE;
   }
   catch(...)
   {
      cerr << "glooper: An unspecified exception has been caught: log file may contain more details" << endl;

      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
