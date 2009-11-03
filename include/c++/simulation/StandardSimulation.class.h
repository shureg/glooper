// =====================================================================================
// 
//       Filename:  StandardSimulation.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 18:56:15
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  STANDARDSIMULATION_CLASS_INC
#define  STANDARDSIMULATION_CLASS_INC

#include "simulation/Simulation.class.h"

namespace GLOOPER_TEST
{
   //! A standard simulation, with fixed number of batches and runs
   class StandardSimulation: public Simulation
   {
   public:

      StandardSimulation(const boost::shared_ptr<Process>&,
	    const std::string&, 
	    const SednaDBInterface&, const char*, 
	    unsigned long, unsigned long);

      const bool end_simulation() const;

      const bool end_batch() const;

      XmlField xml_description() const;

   protected:

      unsigned long N_batches;

      unsigned long N_runs;

   };
}

#endif   // ----- #ifndef STANDARDSIMULATION_CLASS_INC  -----
