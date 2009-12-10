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

   add_registration_data(tmp);

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
      LOG(INFO,boost::format("Beginning batch %d\n") % batch_ctr );
      
      process->batch_config();

      run_ctr = 0;

      while( !end_batch() )
      {
	 LOG(INFO,boost::format("Beginning run %d\n") % run_ctr );

	 process->run_config();

	 step_ctr = 0;

	 registration_timer = 0;

	 dbi.set_autocommit(false);

	 dbi.begin_transaction();

	 while( !end_run() )
	 {
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

void Simulation::add_registration_data(XmlField& xml)
{
   xml("simulation_id") = id;
   xml("batch_id") = batch_ctr;
   xml("run_id") = run_ctr;
   xml("step_id") = step_ctr;
   xml("registration_timer") = ++registration_timer;
}

XmlField Simulation::xml_description() const
{
   XmlField tmp("Simulation");
   tmp("id") = id;
   tmp("comment") = comment;
   tmp("library_version") = LIB_VERSION_STR;

   return tmp;
}
