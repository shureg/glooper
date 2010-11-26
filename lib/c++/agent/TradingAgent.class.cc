// =====================================================================================
// 
//       Filename:  TradingAgent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:57:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/TradingAgent.class.h"
#include <cmath>
#include <iostream>
#include "rng/generator/UniformGenerator.class.h"
#include "boost/bind.hpp"
#include "callback_log/LOG.h"

using namespace GLOOPER_TEST;
using namespace std;
using namespace boost::logic;

TradingAgent::TradingAgent(double _belief, const TRG_d& private_info_generator,
      double _wealth, const TRG_d& spread_generator):
   Agent(_belief,private_info_generator),
   wealth(_wealth), spread_generator( bspTRG_d( spread_generator.clone() ) ),
   is_bankrupt(false), is_overleveraged(false),
   spread_generator_string( spread_generator.xml_description().string_format("qXML_line") )
{
}

double TradingAgent::investment_value(double mtm) const
{
   return ((double) pos)*mtm;
}

double TradingAgent::current_investment_proportion(double mtm) const
{
   double iv = investment_value(mtm);

   double cip = iv/(iv+wealth);

   if( cip < -1.)
      LOG(WARNING, boost::format("Agent %d: the calculated cip(%.4f) is "\
	       "short-leveraged with Q=%d, W=%.0f, mtm=%.3f\n")
	    % id % cip % (double) pos % wealth % mtm
	    );
   if( cip > 1.)
      LOG(ERROR, boost::format("Agent %d: the calculated cip(%.4f) is "\
	       "long-leveraged with Q=%d, W=%.0f, mtm=%.3f\n")
	    % id % cip % (double) pos % wealth % mtm
	    );

   return cip;
}

double TradingAgent::desired_investment_proportion() const
{
   return 2.*(belief-0.5);
}

void TradingAgent::position_update(const Order& r)
{
   bool b = r.is_bid();
   unsigned long q = r.get_quantity();
   double p = r.get_price();

   if(b)
   {
      pos += q;
      wealth -= q*p;
   }
   else
   {
      pos -= q;
      wealth += q*p;
   }
   
   LOG(TRACE, boost::format("Agent %d: an order (%d) for %d units @ %.3f "\
	    "has been executed, the new position is Q=%.0f, W=%.0f\n")
	 % id % b % q % p % (double) pos % wealth
	 );
}

bool TradingAgent::is_active() const
{
   return 2.*abs(belief-0.5) > 0.75;
}

double TradingAgent::spread_fraction() const
{
   return (*spread_generator)();
}

double TradingAgent::bid_ask_spread(double price,bool is_bid) const
{
   if(is_bid)
      return price/(1.+spread_fraction());
   else
      return price*(1.+spread_fraction());
}

tribool TradingAgent::order_is_bid() const
{
   double best_ask = spot_mkt->mark_to_market(true);
   double best_bid = spot_mkt->mark_to_market(false);

   if(best_bid > best_ask)
   {
      LOG(ERROR, boost::format(
	       "TradingAgent::order_is_bid - best bid price %.3f "\
	       "exceeds best ask price %.3f for agent %d, "
	       "preventing any trading activity")
	    % best_bid % best_ask % id
	 );

      return indeterminate;
   }
   else
   {
      double dq_ask = (double) get_order_quantity(best_ask);
      double dq_bid = (double) get_order_quantity(best_bid);

      if(dq_ask > 0)
	 return true;
      else if(dq_bid < 0)
	 return false;
      else
	 return indeterminate;
   }
}

Position TradingAgent::get_order_quantity(double price) const
{
   double q = pos;
   double omega = desired_investment_proportion();
   double dq = (omega/price)*(q*price + wealth) - q;

   LOG(TRACE,boost::format(
	    "Agent %d: with cip= %.4f, dip=%.4f, W=%.0f, Q=%.0f, "\
	    "m=%.3f, the requisite raw order quantity is found to be %f\n")
	 % id % current_investment_proportion(price) 
	 % desired_investment_proportion() 
	 % wealth % q % price % dq
	 );

   if( dq > 0 )
      return Position(floor(dq));
   else if( dq < 0 )
      return Position(ceil(dq));
   else
      return Position(0.);
}

Position TradingAgent::get_order_quantity(double price, 
      Position pos_, double c) const
{
   double q = pos_;
   double omega = desired_investment_proportion();
   double dq = (omega/price)*(q*price + c) - q;

   LOG(TRACE,boost::format(
	    "Agent %d: with cip= %.4f, dip=%.4f, W=%.0f, Q=%.0f, "\
	    "m=%.3f, the requisite raw order quantity is found to be %f\n")
	 % id % current_investment_proportion(price) 
	 % desired_investment_proportion() 
	 % c % q % price % dq
	 );

   if( dq > 0 )
      return Position(floor(dq));
   else if( dq < 0 )
      return Position(ceil(dq));
   else
      return Position(0.);
}

void TradingAgent::place_order()
{
   tribool order_status = order_is_bid();

   tribool prev_order_status;

   if( !indeterminate(order_status) )
   {
      //SimulationObject::db_signal()(*this);

      reset_orders();

      Position dq;

      bool bid = (bool) order_status;

      // Active order
      if( is_active() && spot_mkt->best_order(bid) != 0 )
      {
	 bool stop = false;
	 Position q_ = pos;
	 double c_ = wealth;
	 unsigned long dq_, dq_exec, dq_agg = 0ul;
	 double p_ord;
	 unsigned long q_ord;

	 if(bid)
	 {
	    const set<Order, buyers_pick>& opp = spot_mkt->get_ask_orders();

	    set<Order, buyers_pick>::const_iterator ord = opp.begin();

	    while(!stop && ord != opp.end())
	    {
	       p_ord = ord->get_price();
	       q_ord = ord->get_quantity();

	       dq_ = get_order_quantity(p_ord,q_,c_).q;

	       dq_exec = std::min(dq_,q_ord);

	       dq_agg += dq_exec;
	       q_ += dq_exec;
	       c_ -= dq_exec*p_ord;

	       stop = (dq_ <= q_ord);

	       ++ord;
	    }
	 }
	 else
	 {
	    const set<Order, sellers_pick>& opp = spot_mkt->get_bid_orders();

	    set<Order, sellers_pick>::const_iterator ord = opp.begin();

	    while(!stop && ord != opp.end())
	    {
	       p_ord = ord->get_price();
	       q_ord = ord->get_quantity();

	       dq_ = get_order_quantity(p_ord,q_,c_).q;

	       dq_exec = std::min(dq_,q_ord);

	       dq_agg += dq_exec;
	       q_ -= dq_exec;
	       c_ += dq_exec*p_ord;

	       stop = (dq_ <= q_ord);

	       ++ord;
	    }
	 }

	 Order act(*this, dq_agg, bid, *(*timer)(),"active");

	 spot_mkt->process_order(act);

	 // Run out of opposing side orders and didn't fully execute
	 // Send a limit order for the remaining quantity
	 if(!stop)
	 {
	    Position dq_rem = get_order_quantity(p_ord);

	    if(dq_rem.q != 0)
	       Order rem(*this, p_ord, dq_rem.q, bid, 
		     *(*timer)(),"active_remainder");
	 }

      }
      // Passive order
      else
      {
	 double p_ref = spot_mkt->mark_to_market(bid);
	 double p_ord = spot_mkt->tick_adjusted_price(
				    bid_ask_spread(p_ref,bid) );

	 dq = get_order_quantity(p_ord);

	 if(dq == 0)
	    LOG(WARNING,
		  boost::format(
		     "Agent %d: passive limit order quantity has been "\
		     "calculated to be zero\n"
		     ) % id
		  );
	 else
	 {
	    LOG(TRACE,boost::format("Agent %d: Preparing to execute a passive order "\
		     "for %.0f units at %.2f\n")
		  % id % dq % p_ord
		  );
	    Order pass(*this, p_ord, dq.q, (bool) dq.is_long, 
		  *(*timer)(), "passive");

	    spot_mkt->process_order(pass);
	 }
      }
   }
/*  
   LOG(TRACE, boost::format(
	 "Starting post-trade agent registration process for agent %d\n")
       % id
       );

   SimulationObject::db_signal()(*this);

   LOG(TRACE, boost::format(
	 "Ending post-trade agent registration process for agent %d\n")
       % id
       );
*/
}

void TradingAgent::reset_orders()
{
   spot_mkt->pull_agent_orders(*this);
   LOG(TRACE, boost::format("Agent %d: own orders have been reset\n") % id);
}

double TradingAgent::mark_to_market() const
{
   if(indeterminate(pos.is_long))
   {
     LOG(WARNING, boost::format("Agent %d: the internal mark-to-market method "\
	       "should not be called when there is no position in the asset\n")
	    % id
	    );
     return 0.;
   }
   else
     return spot_mkt->mark_to_market(!pos.is_long);
}

bool TradingAgent::can_trade()
{
   check_wealth();
   if(negative_wealth)
   {
      LOG(WARNING, boost::format(
	       "Agent %d - the cash account is negative at %.2f. "\
	       "Although there are safeguards to make sure this does not cause strange "\
	       "behaviour, they are not universal, and this should not be happening in "\
	       "any case\n") % id % wealth
	    );
   }
   else
   {
      check_leverage();
      check_liquidity();
   }

   return !(negative_wealth || is_bankrupt || is_overleveraged);
}

void TradingAgent::check_wealth()
{
   negative_wealth = (wealth < 0);
}

void TradingAgent::check_liquidity()
{
   if(!indeterminate(pos.is_long))
   {
      LOG(TRACE, boost::format("Agent %d: Beginning the liquidity check\n")
	    % id
	    );

      is_bankrupt = (wealth + investment_value( mark_to_market() ) )  < 0;

      LOG(TRACE, 
	    boost::format("Agent %d: the bankrupcy flag value is now %d\n")
	    % id % is_bankrupt
	 );

   }
}

void TradingAgent::determine_overleverage(double iv, double L)
{
   if(pos.is_long)
      is_overleveraged = ( wealth < iv*(L-1)/L );
   else
      is_overleveraged = ( wealth < -iv*(L+1)/L );
}

void TradingAgent::check_leverage()
{
   if( !indeterminate(pos.is_long) )
   {
      double m = mark_to_market();

      double L = spot_mkt->get_maximum_leverage_factor();

      double iv = investment_value(m);

      determine_overleverage(iv,L);

      if(is_overleveraged)
      {
	 if(pos.is_long)
	 {
	    Order close_long(*this, spot_mkt->gauge_market_depth(true,pos.q),false,*(*timer)(),
		  "deleverage_long");
	    spot_mkt->process_order(close_long);
	 }
	 else
	    spot_mkt->close_out_short(*this,pos.q,iv,*(*timer)());
      }

      determine_overleverage(investment_value( mark_to_market() ), L );

   }
}

XmlField TradingAgent::xml_description() const
{

   double iv, cip;

   if( indeterminate(pos.is_long) )
   {
      iv = 0;
      cip = 0;
   }
   else
   {
      double m = mark_to_market();
      iv = investment_value(m);
      cip = current_investment_proportion(m);
   }

   XmlField tmp = Agent::xml_description();

   tmp("wealth") = wealth;

   tmp("position") = (double) pos;

   tmp("investment_value") = iv;

   tmp("total_investment_value") = iv + wealth;

   tmp("desired_investment_proportion") =
      desired_investment_proportion();

   tmp("current_investment_proportion") = cip;

   tmp("is_bankrupt") = is_bankrupt;

   tmp("is_overleveraged") = is_overleveraged;

   tmp("spread_generator") = spread_generator_string;
   
   return tmp;
}
