// =====================================================================================
// 
//       Filename:  AgentGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/10/09 16:35:35
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/AgentGenerator.class.h"

using namespace GLOOPER_TEST;
using namespace RNG;

AgentGenerator::AgentGenerator(const bsp_TRGd& _belief_generator,
      const bsp_TRGd& _wealth_generator):
   SimulationObject(0), belief_generator(_belief_generator),
   wealth_generator(_wealth_generator)
{}

void AgentGenerator::set_timer(const bsp_ts& _timer)
{
   timer = _timer;
}

void AgentGenerator::set_ro_timer(const bsp_ts& _ro_timer)
{
   ro_timer = _ro_timer;
}

XmlField AgentGenerator::xml_description() const
{
   XmlField tmp("Agent_Generator");
   tmp.add_field("Belief_Generator");
   tmp.add_field("Wealth_Generator");

   tmp["Belief_Generator"].add_field(belief_generator->xml_description());
   tmp["Wealth_Generator"].add_field(wealth_generator->xml_description());

   return tmp;
}
