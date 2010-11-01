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

#include <iostream>

using namespace GLOOPER_TEST;

NoiseTrader::NoiseTrader(double belief, double wealth, const TRG_d& spread_generator,
      const TRG_d& belief_generator_):
   TradingAgent(belief, wealth, spread_generator),
   belief_generator(bspTRG_d(belief_generator_.clone())),
   belief_generator_string( belief_generator_.xml_description().string_format("qXML_line"))
{
}

NoiseTrader::~NoiseTrader()
{
}

void NoiseTrader::update_belief(double)
{
   belief = (*belief_generator)();
}

Agent* NoiseTrader::real_clone() const
{
   NoiseTrader* tmp = new NoiseTrader(*this);

   return (Agent*) tmp;
}

XmlField NoiseTrader::xml_description() const
{
   XmlField tmp = TradingAgent::xml_description();

   tmp("belief_generator") = belief_generator_string;

   return tmp;
}
