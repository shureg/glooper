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
#include "boost/bind.hpp"
#include "boost/function.hpp"

#include <fstream>
#include <algorithm>

using namespace std;

using namespace GLOOPER_TEST;

using XML_SERIALISATION::XmlWrap;
using XML_SERIALISATION::XmlSingleValue;
using CALLBACK_LOG::LOG;

AgentPopulation::AgentPopulation(
      const boost::shared_ptr< TypedRandomGenerator<double> >& _info_generator,
      const boost::shared_ptr< AgentGenerator >& _agt_gen):
   agt_gen(_agt_gen),
   last_info(-1.0), info_generator(_info_generator),
   agent_timer(0), turn_timer(0)
{
   boost::shared_ptr<timer_signal> ts(new timer_signal);
   boost::shared_ptr<timer_signal> ro_ts(new timer_signal);

   ts->connect( boost::bind(&AgentPopulation::get_agent_timer,this,true) );
   ro_ts->connect( boost::bind(&AgentPopulation::get_agent_timer,this,false) );

   agt_gen->set_timer(ts);
   agt_gen->set_ro_timer(ro_ts);
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

   while( !agt_gen->stop_generation() )
      initial_population.push_back( agt_gen->generate_item() );

   XmlWrap ig("Information_Generator",*info_generator);

   SimulationObject::db_signal()(ig);

   SimulationObject::db_signal()(*agt_gen);

   list<XmlWrap> ip = agent_population_description(
	 "initial", initial_population);

   db_container_signal(ip);

}

void AgentPopulation::batch_config() {}

void AgentPopulation::run_config()
{
   turn_timer = 0;

   population.clear();

   for(boost::ptr_vector<Agent>::iterator
	 i  = initial_population.begin();
	 i != initial_population.end();
	 ++i)
   {
      i->get_market()->reset();
      population.push_back( (*i).clone() );
   }
}

void AgentPopulation::evolve()
{
   LOG(INFO,boost::format(
	    "Beginning turn %d\n") % turn_timer);

   last_info = (*info_generator)();

   XmlSingleValue iv("Information","value",last_info,true);
   
   SimulationObject::db_signal()(iv);

   LOG(TRACE,boost::format(
	    "Using information value %f\n") % last_info);

   for(boost::ptr_vector<Agent>::iterator 
	 i = population.begin(); i!=population.end(); ++i)
   {
      i->update_belief(last_info);
      if( i->can_trade() )
	 i->place_order();
   }

   list<XmlWrap> ep = 
      agent_population_description("end_turn",population);

   db_container_signal(ep);

   LOG(INFO,boost::format(
	    "Ending turn %d\n") % turn_timer);

   turn_timer++;

}

list<XmlWrap> AgentPopulation::agent_population_description
   (const std::string& _label,
      const boost::ptr_vector<Agent>& _population) const
{
   list<XmlField> tmp;
   for(boost::ptr_vector<Agent>::const_iterator
	 i = _population.begin(); i != _population.end(); ++i)
   {
      XmlField fld = i->xml_description();
      fld("snapshot_type") = _label;
      tmp.push_back( XmlWrap(tmp) );
   }
   return tmp;
}
