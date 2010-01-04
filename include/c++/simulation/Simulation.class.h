// =====================================================================================
// 
//       Filename:  Simulation.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/10/09 14:04:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  SIMULATION_CLASS_INC
#define  SIMULATION_CLASS_INC

#include "boost/shared_ptr.hpp"
#include "core/SimulationObject.class.h"
#include "db/SednaDBInterface.class.h"
#include "boost/timer.hpp"
#include <vector>

namespace GLOOPER_TEST
{
   class Process;

   class Simulation: public SimulationObject
   {
   public:

      Simulation(const boost::shared_ptr<Process>&,
	    const std::string&, const SednaDBInterface&, const char*);

      Simulation(const boost::shared_ptr<Process>&,
	    const std::string&, const SednaDBInterface&, unsigned long);

      ~Simulation();

      XmlField xml_description() const;

      void simulate();

      virtual const bool end_simulation() const = 0;
      
      virtual const bool end_batch() const = 0;

      virtual const bool end_run() const = 0;

      const std::vector<unsigned long>& get_batch_run_structure() const;

   protected:

      const std::string comment;

      boost::shared_ptr<Process> process;

      SednaDBInterface dbi;

      unsigned long batch_ctr;

      unsigned long run_ctr;

      unsigned long step_ctr;

      unsigned long external_instance_counter(const char*) const;

      std::string current_context;

      unsigned long registration_timer;

      boost::timer* simulation_timer;

      void simulation_cleanup();

      void add_registration_data(XML_SERIALISATION::XmlField&);

      void db_insert_slot(const XML_SERIALISATION::XmlSerialisableObject&);

      std::vector<unsigned long> batch_run_structure;
   };
}

#endif   // ----- #ifndef SIMULATION_CLASS_INC  -----
