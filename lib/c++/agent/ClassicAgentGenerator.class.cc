// =====================================================================================
// 
//       Filename:  ClassicAgentGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 19:56:51
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/ClassicAgentGenerator.class.h"
#include "agent/ClassicAgent.class.h"
#include "xml_serialisation/XmlWrap.class.h"

using namespace GLOOPER_TEST;
using namespace std;
using XML_SERIALISATION::XmlWrap;

ClassicAgentGenerator::ClassicAgentGenerator(
      const bsp_TRGd& _belief_generator, const bsp_TRGd& _wealth_generator,
      const bsp_TRGd& _f_min_gen, const bsp_TRGd& _p_min_gen, 
      const bsp_TRGd& _bas_gen, const boost::shared_ptr<Market>& _mkt, 
      unsigned long _N_agents):
   AgentGenerator(_belief_generator,_wealth_generator),
   f_min_gen(_f_min_gen), p_min_gen(_p_min_gen), bas_gen(_bas_gen),
   mkt(_mkt), N_agents(_N_agents), agt_ctr(0)
{}

Agent* ClassicAgentGenerator::generate_item()
{
   Agent* tmp = (Agent*) new ClassicAgent(
	 (*belief_generator)(), (*p_min_gen)(), (*f_min_gen)(),
	 (*wealth_generator)(), mkt, (*bas_gen)(), timer, ro_timer);

   agt_ctr++;

   return tmp;
}

const bool ClassicAgentGenerator::stop_generation()
{
   return ( agt_ctr >= N_agents );
}

XmlField ClassicAgentGenerator::xml_description() const
{
   XmlField tmp = AgentGenerator::xml_description();

   XmlField c_tmp("Classic_Agent_Generator");

   c_tmp.add_field("agent_count",N_agents);
   c_tmp.add_field( XmlWrap("Minimum_Revision_Probability_Generator",
	 *p_min_gen).xml_description() );
   c_tmp.add_field( XmlWrap("Minimum_Belief_Adjustment_Proportion_Generator",
	 *f_min_gen).xml_description() );
   c_tmp.add_field( XmlWrap("Maximum_Bid-Ask_Spread_Generator",
	 *bas_gen).xml_description() );

   tmp.add_field(c_tmp);

   return tmp;
}
