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
#include "boost/timer.hpp"

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
      FileCallbackLog F("test.log","glooper_test",INFO);

      SednaDBInterface dbi("SimulationDB","Bender Rodriguez","bmsma_DTAH1","SimulationData");

      boost::shared_ptr<UniformGenerator> belief(new UniformGenerator);

      boost::shared_ptr<ParetoGenerator> wealth(new ParetoGenerator(10000,1.4));

      boost::shared_ptr<UniformGenerator> f_min(new UniformGenerator(0.05,0.1));

      boost::shared_ptr<UniformGenerator> p_min(new UniformGenerator(0.05,0.1));

      boost::shared_ptr<UniformGenerator> bas_max(new UniformGenerator(0.001,0.01));

      boost::shared_ptr<Market> mkt(new Market);

      boost::shared_ptr<ClassicAgentGenerator> ag(new ClassicAgentGenerator(belief,wealth,f_min,p_min,bas_max,mkt,2));

      boost::shared_ptr<UniformGenerator> info(new UniformGenerator);

      boost::shared_ptr<AgentPopulation> ap(new AgentPopulation(info,ag));

      ostringstream oss;

      boost::timer t;

      StandardFSSimulation sim(ap,"test",dbi,"simid",1,1,1);

      sim.simulate();

      LOG( INFO, boost::format("Simulation executed in %f\n") % t.elapsed() );
   }
   catch(const char* c)
   {
      cout << "An exception has been caught: " << c << endl;

      return EXIT_FAILURE;
   }
   catch(const string& s)
   {
      cout << "An exception has been caught: " << s << endl;

      return EXIT_FAILURE;
   }
   catch(const std::exception& e)
   {
      cout << "An exception has been caught: " << e.what() << endl;

      return EXIT_FAILURE;
   }
   catch(...)
   {
      cout << "An unspecified exception has been caught: log file may contain more details" << endl;

      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
