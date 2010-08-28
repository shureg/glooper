// =====================================================================================
// 
//       Filename:  StandardFSSimulation.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 19:11:36
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  STANDARDFSSIMULATION_CLASS_INC
#define  STANDARDFSSIMULATION_CLASS_INC

#include "simulation/StandardSimulation.class.h"

namespace GLOOPER_TEST
{
   //! A standard simulation with fixed number of batches, runs and steps 
   class StandardFSSimulation: public StandardSimulation
   {
   public:

      StandardFSSimulation(const boost::shared_ptr<Process>&,
	    const std::string&, 
	    const char*, const char*,
	    const boost::shared_ptr<TableDBInterface>&,
	    unsigned long, unsigned long,
	    unsigned long);

      StandardFSSimulation(const boost::shared_ptr<Process>&,
	    const std::string&, 
	    const char*, unsigned long,
	    const boost::shared_ptr<TableDBInterface>&,
	    unsigned long, unsigned long,
	    unsigned long);

      const bool end_run() const;

      XmlField xml_description() const;

   protected:

      unsigned long N_steps;
   };
}

#endif   // ----- #ifndef STANDARDFSSIMULATION_CLASS_INC  -----
