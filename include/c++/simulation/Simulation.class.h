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
#include "xml_serialisation/XmlFile.class.h"
#include <vector>

namespace GLOOPER_TEST
{
   class Process;

   class Simulation: public SimulationObject
   {
   public:

      Simulation(const boost::shared_ptr<Process>&,
	    const std::string&, const char*, const char*);

      Simulation(const boost::shared_ptr<Process>&,
	    const std::string&, const char*, unsigned long);

      ~Simulation();

      XmlField xml_description() const;

      void simulate();

      virtual const bool end_simulation() const = 0;
      
      virtual const bool end_batch() const = 0;

      virtual const bool end_run() const = 0;

      const std::vector<unsigned long>& get_batch_run_structure() const;

   protected:

      const std::string comment;

      const char* base_path;

      boost::shared_ptr<Process> process;

      unsigned long batch_ctr;

      unsigned long run_ctr;

      unsigned long step_ctr;

      XML_SERIALISATION::XmlFile sim_file;

      XML_SERIALISATION::XmlFile batch_file;

      XML_SERIALISATION::XmlFile run_file;

      XML_SERIALISATION::XmlFile step_file;

      unsigned long external_instance_counter(const char*) const;

      unsigned long registration_timer;

      XML_SERIALISATION::XmlFile* current_file;

      void simulation_cleanup();

      void db_insert_slot(const XML_SERIALISATION::XmlSerialisableObject&);

      std::vector<unsigned long> batch_run_structure;

   private:

      const std::string sim_string() const;

      const std::string batch_string() const;

      const std::string run_string() const;

      const std::string step_string() const;

   };
}

#endif   // ----- #ifndef SIMULATION_CLASS_INC  -----
