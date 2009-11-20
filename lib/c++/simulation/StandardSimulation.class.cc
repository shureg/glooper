// =====================================================================================
// 
//       Filename:  StandardSimulation.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 18:59:21
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "simulation/StandardSimulation.class.h"

using namespace GLOOPER_TEST;
using namespace std;

StandardSimulation::StandardSimulation(
      const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const SednaDBInterface& _dbi, const char* _efn,
      unsigned long _N_batches, unsigned long _N_runs):
   Simulation(_process,_comment,_dbi,_efn), 
   N_batches(_N_batches), N_runs(_N_runs)
{}

StandardSimulation::StandardSimulation(
      const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const SednaDBInterface& _dbi, unsigned long _id,
      unsigned long _N_batches, unsigned long _N_runs):
   Simulation(_process,_comment,_dbi,_id), 
   N_batches(_N_batches), N_runs(_N_runs)
{}

const bool StandardSimulation::end_simulation() const
{
   return ( batch_ctr >= N_batches );
}

const bool StandardSimulation::end_batch() const
{
   return ( run_ctr >= N_runs );
}

XmlField StandardSimulation::xml_description() const
{
   XmlField tmp = Simulation::xml_description();

   tmp.add_field("planned_batch_count",N_batches);
   tmp.add_field("planned_run_count",N_runs);

   return tmp;
}
