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

using namespace GLOOPER_TEST;

unsigned long Agent::instance_counter(0);

Agent::Agent(double _belief,
     boost::shared_ptr<Market> _spot_mkt,
     boost::shared_ptr<timer_signal> _timer,
     boost::shared_ptr<timer_signal> _ro_timer):
   SimulationObject(++instance_counter),
   spot_mkt(_spot_mkt),
   belief(_belief),
   timer(_timer), ro_timer(_ro_timer)
{}

Agent* Agent::clone() const
{
   return real_clone();
}

boost::shared_ptr<Market> Agent::get_market() const
{
   return spot_mkt;
}

XmlField Agent::xml_description() const
{
   XmlField tmp("Agent");
   tmp("id")=id;
   tmp("belief") = belief;
   tmp("timer") = (*ro_timer)();

   return tmp;
}
