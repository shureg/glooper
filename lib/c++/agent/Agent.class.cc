// =====================================================================================
// 
//       Filename:  Agent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:48:40
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/Agent.class.h"
#include "boost/make_shared.hpp"

using boost::make_shared;

using namespace GLOOPER_TEST;

unsigned long Agent::instance_counter(0);

Agent::Agent(double _belief):
   SimulationObject(++instance_counter),
   belief(_belief)
{}

Agent* Agent::clone() const
{
   return real_clone();
}

boost::shared_ptr<Market> Agent::get_market() const
{
   return spot_mkt;
}

Agent& Agent::set_market(const boost::shared_ptr<Market>& mkt)
{
   spot_mkt = mkt;
   return *this;
}

Agent& Agent::set_timer(const boost::shared_ptr<timer_signal>& _timer)
{
   timer = _timer;
   return *this;
}

Agent& Agent::set_ro_timer(const boost::shared_ptr<timer_signal>& _ro_timer)
{
   ro_timer = _ro_timer;
   return *this;
}

XmlField Agent::xml_description() const
{
   XmlField tmp("Agent");

   LOG(TRACE, boost::format("Recording id for agent %d\n") % id);

   tmp("id") = id;

   LOG(TRACE, boost::format("Recording type for agent %d\n") % id);

   tmp("type") = agent_type_str();
   
   LOG(TRACE, boost::format("Recording belief for agent %d\n") % id);

   tmp("belief") = belief;
   
   LOG(TRACE, boost::format("Recording timer for agent %d\n") % id);

   tmp("timer") = (*ro_timer)();

   return tmp;
}
