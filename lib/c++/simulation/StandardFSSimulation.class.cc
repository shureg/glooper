// =====================================================================================
// 
//       Filename:  StandardFSSimulation.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 19:11:40
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "simulation/StandardFSSimulation.class.h"

using namespace GLOOPER_TEST;
using namespace std;

StandardFSSimulation::StandardFSSimulation(
      const boost::shared_ptr<Process>& _process,
      const string& _comment,
      const SednaDBInterface& _dbi, const char* _efn,
      unsigned long _N_batches, unsigned long _N_runs,
      unsigned long _N_steps):
   StandardSimulation(_process,_comment,_dbi,_efn,_N_batches,_N_runs),
   N_steps(_N_steps)
{}

const bool StandardFSSimulation::end_run() const
{
   return ( step_ctr >= N_steps );
}

XmlField StandardFSSimulation::xml_description() const
{
   XmlField tmp = StandardSimulation::xml_description();

   tmp.add_field("planned_step_count",N_steps);

   return tmp;
}
