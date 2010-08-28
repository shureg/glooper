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
#include "boost/scoped_ptr.hpp"
#include "rng/rng.hpp"
#include "boost/make_shared.hpp"
#include "boost/assign.hpp"

using boost::make_shared;

using namespace GLOOPER_TEST;
using namespace std;
using XML_SERIALISATION::XmlSerialisableObject;
using XML_SERIALISATION::XmlFile;

boost::mutex Simulation::run_ctr_mutex;

Simulation::run_space::run_space():
   registration_timer(0ul), step_ctr(0ul)
{}

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

   if(runs_executing)
   {
      ++(current_run->registration_timer);
      current_run->tdb->get_external_fields()[4] =
	 current_tbl_context(4,4)[4];
   }
   else
   {
      sim_file.insert(tmp);
   }

   current_run->tdb->process(tmp);
}

void Simulation::connect_db_signal()
{
   SimulationObject::db_signal().connect( 
	 boost::bind(&Simulation::db_insert_slot,this,_1) );
}

Simulation::Simulation(const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const char* base_path,
      const boost::shared_ptr<TableDBInterface>& tdb,
      const char* external_counter_filename): 
   SimulationObject(external_instance_counter(external_counter_filename)),
   comment(_comment), base_path(base_path), process(_process),
   tdb_init(tdb),
   batch_ctr(0), run_ctr(0),
   N_threads(0), runs_executing(false),
   total_step_count(0)
{

   sim_file.assign(sim_string(),"SimulationData");

   current_run.reset( new Simulation::run_space );

   tdb_init->init( boost::assign::list_of
	 ("sim_id")("batch_id")("run_id")("step_id")("registration_timer"),
	 (boost::format("%d") % id).str() );

   current_run->tdb = make_shared<TableDBInterface>(*tdb_init);

   connect_db_signal();

   SimulationObject::db_signal()(*this);
}

Simulation::Simulation(const boost::shared_ptr<Process>& _process,
      const std::string& _comment,
      const char* base_path,
      const boost::shared_ptr<TableDBInterface>& tdb,
      unsigned long _id): 
   SimulationObject(_id),
   comment(_comment), base_path(base_path), process(_process),
   tdb_init(tdb),
   batch_ctr(0), run_ctr(0),
   N_threads(0), runs_executing(false),
   total_step_count(0)
{

   sim_file.assign(sim_string(),"SimulationData");

   current_run.reset( new Simulation::run_space );

   tdb_init->init( boost::assign::list_of
	 ("sim_id")("batch_id")("run_id")("step_id")("registration_timer"),
	 (boost::format("%d") % id).str() );

   current_run->tdb = make_shared<TableDBInterface>(*tdb_init);

   connect_db_signal();

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
   return ( boost::format("%s/sim.%d.%03d.xml") % base_path % id % batch_ctr ).str();
}

const string Simulation::run_string(unsigned long rc) const
{
   return ( boost::format("%s/sim.%d.%03d.%03d.xml") % base_path % id % batch_ctr % rc ).str();
}

const string Simulation::step_string(unsigned long rc) const
{
   return ( boost::format("%s/sim.%d.%03d.%03d.%05d.xml") 
	 % base_path % id % batch_ctr % rc % 
	 (current_run->step_ctr) ).str();
}

unsigned long Simulation::increment_run_ctr()
{
   boost::lock_guard<boost::mutex> run_ctr_lock(run_ctr_mutex);

   return run_ctr++;
}

void Simulation::do_runs()
{
   LOG(INFO,boost::format("Beginning run execution in thread %s\n")
	 % (boost::this_thread::get_id())
	 );

   log_ptr->spawn_threaded_log();

   if( current_run.get() == 0 )
      current_run.reset( new Simulation::run_space );

   current_run->tdb = make_shared<TableDBInterface>(*tdb_init);

   SimulationObject::init_sig_ptr();

   connect_db_signal();

   RNG::init();

   current_run->process = boost::shared_ptr<Process>(
	 this->process->clone() );

   current_run->process->thread_config();

   while( !end_batch() )
   {
      unsigned long local_run_ctr = increment_run_ctr();

      //log_ptr->set_context((boost::format("%s.%s.%s") 
	       //% id % batch_ctr % local_run_ctr).str());

      current_run->tdb->transaction_begin( 
	    current_tbl_context(2, 0, local_run_ctr) );

      LOG(INFO,boost::format("Beginning run %d\n") % local_run_ctr );

      current_run->process->run_config();

      current_run->step_ctr = 0;

      current_run->registration_timer = 0;

      while( !end_run() )
      {
	 //log_ptr->set_context((boost::format("%s.%s.%s.%s") 
		  //% id % batch_ctr % local_run_ctr % 
		  //current_run->step_ctr).str());

	 current_run->tdb->get_external_fields()[3] =
	    current_tbl_context(3,3)[3];

	 LOG(TRACE,boost::format(
		  "Evolving process at %0x on thread %s\n")
	       % (current_run->process) % 
	       (boost::this_thread::get_id())
	       );

	 current_run->process->evolve();

	 ++(current_run->step_ctr);
      }

      //log_ptr->set_context((boost::format("%s.%s.%s") 
	       //% id % batch_ctr % local_run_ctr).str());

      update_total_step_count(current_run->step_ctr);

      LOG(INFO,boost::format("Ending run %d\n") % local_run_ctr );

      current_run->tdb->transaction_end();
   }

   LOG(INFO,boost::format("Ending run execution in thread %s\n")
	 % (boost::this_thread::get_id())
	 );
}

void Simulation::simulate()
{
   //log_ptr->set_context((boost::format("%s") % id).str());

   current_run->tdb->transaction_begin( current_tbl_context() );

   process->simulation_config();

   current_run->tdb->transaction_end();

   batch_ctr = 0;

   while( !end_simulation() )
   {
      //log_ptr->set_context((boost::format("%s.%s") 
	  //     % id % batch_ctr).str());

      LOG(INFO,boost::format("Beginning batch %d\n") % batch_ctr );
      
      process->batch_config();

      run_ctr = 0;

      runs_executing = true;

      for(unsigned long t = 0; t < N_threads; ++t)
	 runs.create_thread( boost::bind( &Simulation::do_runs,this ) );

      runs.join_all();

      //log_ptr->set_context((boost::format("%s.%s") 
	//       % id % batch_ctr).str());

      runs_executing = false;

      LOG(INFO,boost::format("Ending batch %d\n") % batch_ctr );

      batch_run_structure.push_back(run_ctr);

      ++batch_ctr;
   }

   //log_ptr->set_context((boost::format("%s") % id).str());

   simulation_cleanup();

   sim_file.commit();
}

const vector<unsigned long>& Simulation::get_batch_run_structure() const
{
   return batch_run_structure;
}

void Simulation::update_total_step_count(unsigned long steps)
{
   boost::lock_guard<boost::mutex> total_step_lock(total_step_count_mutex);

   total_step_count += steps;
}

void Simulation::simulation_cleanup()
{
}

vector<string> Simulation::current_tbl_context(
      unsigned long lvl1, unsigned long lvl0, unsigned long rc) const
{
   vector<string> tmp(lvl1+1);

   if(lvl0 < 1)
      tmp[0] = (boost::format("%d") % id).str();
   if(lvl1 > 0 && lvl0 < 2)
      tmp[1] = (boost::format("%03d") % batch_ctr).str();
   if(lvl1 > 1 && lvl0 < 3)
      tmp[2] = (boost::format("%03d") % rc).str();
   if(lvl1 > 2 && lvl0 < 4)
      tmp[3] = (boost::format("%05d") % current_run->step_ctr).str();
   if(lvl1 > 3 && lvl0 < 5)
      tmp[4] = (boost::format("%07d") 
	    % current_run->registration_timer).str();

   return tmp;
}

void Simulation::set_N_threads(const unsigned long _N_threads)
{
   N_threads = _N_threads;
}

void Simulation::set_log_ptr(const boost::shared_ptr<CallbackLog>& _log_ptr)
{
   log_ptr = _log_ptr;
}

unsigned long Simulation::get_total_step_count() const
{
   return total_step_count;
}

XmlField Simulation::xml_description() const
{
   XmlField tmp("Simulation");
   tmp("id") = id;
   tmp("comment") = comment;
   tmp("library_version") = LIB_VERSION_STR;

   return tmp;
}
