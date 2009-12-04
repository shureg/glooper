// =====================================================================================
// 
//       Filename:  Simulation.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/10/09 14:04:42
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "simulation/Simulation.class.h"
#include "process/Process.class.h"
#include <fstream>
#include <sstream>
#include "boost/bind.hpp"
#include "core/version.h"

using namespace GLOOPER_TEST;
using namespace std;
using XML_SERIALISATION::XmlSerialisableObject;

unsigned long Simulation::external_instance_counter(const char* filename) const
{
   unsigned long tmp;

   ifstream ifs(filename);

   ifs >> tmp;

   ifs.close();

   ofstream ofs(filename);

   ofs << ++tmp;

   ofs.close();

   return tmp;
}

void Simulation::db_insert_slot(const XmlSerialisableObject& so)
{
   ostringstream oss;

   XmlField tmp = so.xml_description();

   tmp("registration_timer") = ++registration_timer;

   oss << tmp;

   dbi.insert(oss.str(),current_context);
}

Simulation::Simulation(const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const SednaDBInterface& _dbi,
      const char* external_counter_filename): 
   SimulationObject(external_instance_counter(external_counter_filename)),
   comment(_comment), process(_process), dbi(_dbi),
   batch_ctr(0), run_ctr(0), step_ctr(0),
   registration_timer(0), simulation_timer(0)
{
   dbi.open_connection();

   SimulationObject::db_signal().connect( 
	 boost::bind(&Simulation::db_insert_slot,this,_1) );

   current_context = dbi.default_context();

   SimulationObject::db_signal()(*this);
}

Simulation::Simulation(const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const SednaDBInterface& _dbi,
      unsigned long _id): 
   SimulationObject(_id),
   comment(_comment), process(_process), dbi(_dbi),
   batch_ctr(0), run_ctr(0), step_ctr(0),
   registration_timer(0), simulation_timer(0)
{
   dbi.open_connection();

   dbi.set_autocommit(true);

   SimulationObject::db_signal().connect( 
	 boost::bind(&Simulation::db_insert_slot,this,_1) );

   current_context = dbi.default_context();

   SimulationObject::db_signal()(*this);
}

Simulation::~Simulation()
{
   delete simulation_timer;
}

void Simulation::simulate()
{
   dbi.set_autocommit(true);

   simulation_timer = new boost::timer;

   string simulation_context = 
      dbi.default_context() + 
      (boost::format("/Simulation[@id=%d]") % id).str();
   
   current_context = simulation_context;

   process->simulation_config();

   batch_ctr = 0;

   while( !end_simulation() )
   {
      XmlField batch("Batch");
      batch("id") = batch_ctr;
      batch("context_id") = (boost::format("%d.%d") % id % batch_ctr).str();

      dbi.insert((const string) batch,simulation_context);

      string batch_context = simulation_context +
	 (boost::format("/Batch[@id=%d]") % batch_ctr).str();

      current_context = batch_context;

      LOG(INFO,boost::format("Beginning batch %d\n") % batch_ctr );
      
      process->batch_config();

      run_ctr = 0;

      while( !end_batch() )
      {
	 XmlField run("Run");
	 run("id") = run_ctr;
	 run("context_id") = 
	    (boost::format("%d.%d.%d") % id % batch_ctr % run_ctr).str();

	 dbi.insert((const string) run, batch_context);

	 string run_context = batch_context +
	    (boost::format("/Run[@id=%d]") % run_ctr).str();

	 current_context = run_context;

	 LOG(INFO,boost::format("Beginning run %d\n") % run_ctr );

	 process->run_config();

	 step_ctr = 0;

	 registration_timer = 0;

	 dbi.set_autocommit(false);

	 dbi.begin_transaction();

	 while( !end_run() )
	 {
	    XmlField step("Step");
	    step("id") = step_ctr;
	    step("context_id") = 
	       (boost::format("%d.%d.%d.%d") 
	       % id % batch_ctr % run_ctr % step_ctr).str(); 

	    dbi.insert((const string) step, run_context);

	    string step_context = 
	       (boost::format(
	       "index-scan(\"step_context\",\"%s\",\"EQ\")") 
		% (const string) step("context_id")
		  ).str();

	    current_context = step_context;

	    process->evolve();

	    ++step_ctr;
	 }

	 dbi.commit_transaction();

	 dbi.set_autocommit(true);

	 LOG(INFO,boost::format("Ending run %d\n") % run_ctr );

	 ++run_ctr;
      }

      LOG(INFO,boost::format("Ending batch %d\n") % batch_ctr );

      ++batch_ctr;
   }

   current_context = simulation_context;

   simulation_cleanup();
}

void Simulation::simulation_cleanup()
{
   XmlField te( "simulation_time", simulation_timer->elapsed() );
   dbi.insert((const string) te, current_context);
}

XmlField Simulation::xml_description() const
{
   XmlField tmp("Simulation");
   tmp("id") = id;
   tmp("comment") = comment;
   tmp("library_version") = LIB_VERSION_STR;

   return tmp;
}
