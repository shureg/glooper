// =====================================================================================
// 
//       Filename:  AgentPopulation.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:34:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "process/AgentPopulation.class.h"
#include "agent/Agent.class.h"
#include "callback_log/LOG.h"
#include "xml_serialisation/XmlSingleValue.class.hpp"
#include "xml_serialisation/XmlWrap.class.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "rng/algorithm/shuffle.hpp"

#include <fstream>
#include <algorithm>

using namespace std;

using namespace GLOOPER_TEST;

using XML_SERIALISATION::XmlWrap;
using XML_SERIALISATION::XmlContainerWrap;
using XML_SERIALISATION::XmlSingleValue;
using CALLBACK_LOG::LOG;

AgentPopulation::AgentPopulation(
      const boost::shared_ptr< InfoGenerator >& info_generator,
      const boost::shared_ptr< AgentGenerator >& agt_gen,
      const boost::shared_ptr< Market >& mkt):
   agt_gen(agt_gen),
   last_info(-1.0), info_generator(info_generator),
   mkt(mkt),
   agent_timer(0), turn_timer(0)
{
}

unsigned long AgentPopulation::get_agent_timer(bool increase)
{
   if(increase)
      return ++agent_timer;
   else
      return agent_timer;
}

void AgentPopulation::simulation_config()
{
   if( !agt_gen )
      LOG(EXCEPTION, boost::format(
	       "No AgentGenerator set for the Agent Population object\n") 
	    );

   agt_gen->generate();

   initial_population = agt_gen->get_agt_vector().clone();

   boost::shared_ptr<timer_signal> ts(new timer_signal);
   boost::shared_ptr<timer_signal> ro_ts(new timer_signal);

   ts->connect( boost::bind(&AgentPopulation::get_agent_timer,this,true) );
   ro_ts->connect( boost::bind(&AgentPopulation::get_agent_timer,this,false) );

   population_index.clear();

   unsigned int p_ix = 0;

   for( boost::ptr_vector<Agent>::iterator 
	 i = initial_population.begin();
	 i != initial_population.end();
	 ++i)
   {
      (*i).set_market(mkt).set_timer(ts).set_ro_timer(ro_ts);
      (*i).init();
      population_index.push_back(p_ix++);
   }

   SimulationObject::db_signal()(*info_generator);

   SimulationObject::db_signal()(*agt_gen);

   XmlContainerWrap ip = agent_population_description(
	 "Initial.Agent.Population", initial_population);

   SimulationObject::db_signal()(ip);
}

void AgentPopulation::batch_config() {}

void AgentPopulation::run_config()
{
   turn_timer = 0;

   agent_timer = 0;

   population.clear();

   mkt->reset();

   info_generator->reset();

   population = initial_population.clone();
}

void AgentPopulation::evolve()
{
   LOG(INFO,boost::format(
	    "Beginning turn %d\n") % turn_timer);

   last_info = info_generator->get_info(turn_timer);

   XmlSingleValue iv("Information","value",last_info,true);
   
   SimulationObject::db_signal()(iv);

   LOG(TRACE,boost::format(
	    "Using information value %f\n") % last_info);

   RNG::knuth_shuffle(population_index.begin(),population_index.end());

   for(std::vector<unsigned int>::iterator 
	 i = population_index.begin(); i != population_index.end(); ++i)
   {
      if(last_info >= 0. && last_info <= 1.)
   population[*i].update_belief(last_info);
      if( population[*i].can_trade() )
	 population[*i].place_order();
   }

   XmlContainerWrap ep = 
      agent_population_description(
	    "Step.End.Agent.Population",population);

   SimulationObject::db_signal()(ep);

   LOG(INFO,boost::format(
	    "Ending turn %d\n") % turn_timer);

   turn_timer++;

}

XmlContainerWrap AgentPopulation::agent_population_description
   (const std::string& _label,
      const boost::ptr_vector<Agent>& _population) const
{
   XmlContainerWrap tmp(_label);
   for(boost::ptr_vector<Agent>::const_iterator
	 i = _population.begin(); i != _population.end(); ++i)
   {
      tmp.add_item( *i );
   }
   return tmp;
}
