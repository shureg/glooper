// =====================================================================================
// 
//       Filename:  HomogeneousAgent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  16/05/10 11:30:05
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/HomogeneousAgent.class.h"
#include "trading/Order.class.h"

using GLOOPER_TEST::HomogeneousAgent;
using GLOOPER_TEST::Agent;

HomogeneousAgent::HomogeneousAgent(double belief, 
      unsigned long order_size, 
      double min_spread, double max_spread):
   Agent(belief), order_size(order_size), 
   min_spread(min_spread), max_spread(max_spread),
   spread_gen(RNG::UniformGenerator(min_spread,max_spread))
{}

void HomogeneousAgent::update_belief(double xi)
{
   double a = std::min(belief, xi);
   double b = std::max(belief, xi);

   belief = U()*(b-a) + a;
}

void HomogeneousAgent::place_order()
{
   if (belief != 0.5)
   {
      bool bid = (belief > 0.5);

      bool active;
      double trade_prob;

      if (belief < 0.25 || belief > 0.75)
      {
	 active = true;
	 if(bid)
	    trade_prob = (belief - 0.75)*4;
	 else
	    trade_prob = (0.25 - belief)*4;
      }
      else
      {
	 active = false;
	 trade_prob = fabs(belief-0.5)*4;
      }

      LOG(TRACE, boost::format(
	       "Homogeneous agent %d: with belief %.4f, "\
	       "the direction flags is (%d), the active flag is (%d), "\
	       "and the trade probability is %.4f\n")
	    % id % belief % bid % active % trade_prob
	    );

      const Order* best = spot_mkt->best_order(bid);

      bool trade_test = ( U() < trade_prob );

      if(trade_test)
      {
	 if (active && best != 0)
	 {
	    LOG(TRACE, boost::format(
		     "Homogeneous agent %d: placing a market order "\
		     "for %d units\n")
		  % id % order_size
		  );

	    Order mkt( *this, order_size, bid, *(*timer)() );
	    
	    spot_mkt->process_order(mkt);
	 }
	 else
	 {
	    double spf = ( bid ) ? 
	       ( 1./(1.+spread_gen()) ) : ( 1. + spread_gen() );

	    double p_ref = spot_mkt->mark_to_market( bid );
	    double p_ord = spot_mkt->tick_adjusted_price( spf*p_ref );

	    LOG(TRACE, boost::format(
		     "Homogeneous agent %d: placing a limit order "\
		     "for %d units @ %.2f\n")
		  % id % order_size % p_ord
		  );

	    Order lim( *this, p_ord, order_size, bid, *(*timer)() );
	    
	    spot_mkt->process_order(lim);
	 }
      }
   }
}

Agent* HomogeneousAgent::real_clone() const
{
   HomogeneousAgent* tmp = new HomogeneousAgent(*this);
   return (Agent*) tmp;
}

XmlField HomogeneousAgent::xml_description() const
{
   XmlField tmp = Agent::xml_description();

   tmp("order_size") = order_size;
   tmp("min_spread") = min_spread;
   tmp("max_spread") = max_spread;

   return tmp;
}
