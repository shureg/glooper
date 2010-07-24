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
using XML_SERIALISATION::XmlFile;

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
   XmlField tmp = so.xml_description();

   tmp("registration_timer") = ++registration_timer;

   if( current_file == 0 )
      LOG(EXCEPTION, boost::format("output XmlFile object not set\n") );

   current_file->insert(tmp);
}

Simulation::Simulation(const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const char* base_path,
      const char* external_counter_filename): 
   SimulationObject(external_instance_counter(external_counter_filename)),
   comment(_comment), base_path(base_path), process(_process),
   batch_ctr(0), run_ctr(0), step_ctr(0),
   registration_timer(0), current_file(0)
{

   sim_file.assign(sim_string(),"Simulation");

   SimulationObject::db_signal().connect( 
	 boost::bind(&Simulation::db_insert_slot,this,_1) );

   current_file = &sim_file;

   SimulationObject::db_signal()(*this);
}

Simulation::Simulation(const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const char* base_path,
      unsigned long _id): 
   SimulationObject(_id),
   comment(_comment), base_path(base_path), process(_process),
   batch_ctr(0), run_ctr(0), step_ctr(0),
   registration_timer(0), current_file(0)
{

   sim_file.assign(sim_string(),"Simulation");

   SimulationObject::db_signal().connect( 
	 boost::bind(&Simulation::db_insert_slot,this,_1) );

   current_file = &sim_file;

   SimulationObject::db_signal()(*this);
}

Simulation::~Simulation()
{
}

const string Simulation::sim_string() const
{
   return ( boost::format("%s/sim.%d.xml") % base_path % id ).str();
}

const string Simulation::batch_string() const
{
   return ( boost::format("%s/sim.%d.%d.xml") % base_path % id % batch_ctr ).str();
}

const string Simulation::run_string() const
{
   return ( boost::format("%s/sim.%d.%d.%d.xml") % base_path % id % batch_ctr % run_ctr ).str();
}

const string Simulation::step_string() const
{
   return ( boost::format("%s/sim.%d.%d.%d.%d.xml") 
	 % base_path % id % batch_ctr % run_ctr % step_ctr ).str();
}

void Simulation::simulate()
{
   current_file = &sim_file;

   process->simulation_config();

   batch_ctr = 0;

   while( !end_simulation() )
   {
      batch_file.assign(batch_string(),"Batch");

      current_file = &batch_file;

      LOG(INFO,boost::format("Beginning batch %d\n") % batch_ctr );
      
      process->batch_config();

      run_ctr = 0;

      while( !end_batch() )
      {
	 run_file.assign(run_string(),"Run");

	 current_file = &run_file;

	 LOG(INFO,boost::format("Beginning run %d\n") % run_ctr );

	 process->run_config();

	 step_ctr = 0;

	 registration_timer = 0;

	 while( !end_run() )
	 {
	    step_file.assign(step_string(),"Step");

	    current_file = &step_file;

	    process->evolve();

	    step_file.commit();

	    ++step_ctr;
	 }

	 LOG(INFO,boost::format("Ending run %d\n") % run_ctr );

	 run_file.commit();

	 ++run_ctr;
      }

      LOG(INFO,boost::format("Ending batch %d\n") % batch_ctr );

      batch_run_structure.push_back(run_ctr);

      batch_file.commit();

      ++batch_ctr;
   }

   current_file = &sim_file;

   simulation_cleanup();

   sim_file.commit();
}

const vector<unsigned long>& Simulation::get_batch_run_structure() const
{
   return batch_run_structure;
}

void Simulation::simulation_cleanup()
{
}

XmlField Simulation::xml_description() const
{
   XmlField tmp("Simulation");
   tmp("id") = id;
   tmp("comment") = comment;
   tmp("library_version") = LIB_VERSION_STR;

   return tmp;
}
