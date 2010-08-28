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
#include "boost/thread.hpp"
#include "callback_log/CallbackLog.class.h"
#include "db/TableDBInterface.class.h"

namespace GLOOPER_TEST
{
   class Process;

   class Simulation: public SimulationObject
   {
   public:

      Simulation(const boost::shared_ptr<Process>&,
	    const std::string&, const char*,
	    const boost::shared_ptr<TableDBInterface>&,
	    const char*);

      Simulation(const boost::shared_ptr<Process>&,
	    const std::string&, const char*,
	    const boost::shared_ptr<TableDBInterface>&,
	    unsigned long);

      ~Simulation();

      XmlField xml_description() const;

      void simulate();

      virtual const bool end_simulation() const = 0;
      
      virtual const bool end_batch() const = 0;

      virtual const bool end_run() const = 0;

      const std::vector<unsigned long>& get_batch_run_structure() const;

      void set_N_threads(const unsigned long);

      void set_log_ptr(const boost::shared_ptr<CallbackLog>&);

      unsigned long get_total_step_count() const;

   protected:

      class run_space
      {
      public:

	 run_space();

	 unsigned long registration_timer;

	 unsigned long step_ctr;

	 boost::shared_ptr<Process> process;

	 boost::shared_ptr<TableDBInterface> tdb;

      };

      boost::thread_specific_ptr<Simulation::run_space>
	 current_run;

      const std::string comment;

      const char* base_path;

      boost::shared_ptr<Process> process;

      boost::shared_ptr<TableDBInterface> tdb_init;

      unsigned long batch_ctr;

      unsigned long run_ctr;

      XML_SERIALISATION::XmlFile sim_file;

      unsigned long external_instance_counter(const char*) const;

      void simulation_cleanup();

      void db_insert_slot(const XML_SERIALISATION::XmlSerialisableObject&);

      std::vector<unsigned long> batch_run_structure;

      unsigned long N_threads;

      boost::shared_ptr<CallbackLog> log_ptr;

      void do_runs();

      static boost::mutex run_ctr_mutex;

      boost::thread_group runs;

      bool runs_executing;

      std::vector<std::string> current_tbl_context(
	    unsigned long lvl1 = 0, 
	    unsigned long lvl0 = 0,unsigned long rc = 0) const;

      unsigned long total_step_count;

      boost::mutex total_step_count_mutex;

      void update_total_step_count(unsigned long);

   private:

      const std::string sim_string() const;

      const std::string batch_string() const;

      const std::string run_string(unsigned long) const;

      const std::string step_string(unsigned long) const;

      void connect_db_signal();

      unsigned long increment_run_ctr();

   };
}

#endif   // ----- #ifndef SIMULATION_CLASS_INC  -----
