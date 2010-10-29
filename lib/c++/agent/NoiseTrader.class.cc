// =====================================================================================
// 
//       Filename:  NoiseTrader.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/26/2010 08:50:13 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/NoiseTrader.class.h"

using namespace GLOOPER_TEST;

NoiseTrader::NoiseTrader(double belief, double wealth,
      const TRG_d& belief_generator_, double max_bid_ask_sp):
   TradingAgent(belief, wealth),
   belief_generator(bspTRG_d(belief_generator_.clone())),
   max_bid_ask_sp(max_bid_ask_sp), 
   U( RNG::UniformGenerator( max_bid_ask_sp/10, max_bid_ask_sp ) )
{}

void NoiseTrader::update_belief(double)
{
   belief = (*belief_generator)();
}

double NoiseTrader::spread_fraction() const
{
   return U();
}

Agent* NoiseTrader::real_clone() const
{
   NoiseTrader* tmp = new NoiseTrader(*this);

   return (Agent*) tmp;
}

XmlField NoiseTrader::xml_description() const
{
   XmlField tmp = TradingAgent::xml_description();

   tmp("nt_max_bid_ask") = max_bid_ask_sp;

   XmlField bg = belief_generator->xml_description();

   tmp("belief_generator_short") = bg.string_format("Python");

   tmp.add_field("Belief.Generator");

   tmp["Belief.Generator"] = bg;

   return tmp;
}
