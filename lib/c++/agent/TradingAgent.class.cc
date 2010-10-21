// =====================================================================================
// 
//       Filename:  ClassicAgent.class.cc
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

#include "agent/ClassicAgent.class.h"
#include <cmath>
#include <iostream>
#include "rng/generator/UniformGenerator.class.h"
#include "boost/bind.hpp"
#include "callback_log/LOG.h"

using namespace GLOOPER_TEST;
using namespace std;
using namespace boost::logic;

TradingAgent::TradingAgent(double _belief,
      double _wealth):
   Agent(_belief),
   wealth(_wealth), is_bankrupt(false), is_overleveraged(false), force_passive(false)
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
   return (!force_passive) && ( 2.*abs(belief-0.5) > 0.75 );
}

double TradingAgent::bid_ask_spread(double price,bool is_bid) const
{
   if(is_bid)
      return price/(1.+spread_fraction());
   else
      return price*(1.+spread_fraction());
}

tribool TradingAgent::illiquid_market_order_status(double dip) const
{
   const Order* best_ask = spot_mkt->best_order(true);
   const Order* best_bid = spot_mkt->best_order(false);

   if(best_ask == 0 && best_bid == 0)
   {
      force_passive = true;
      return indeterminate;
   }
   else if(best_ask == 0 && best_bid != 0)
   {
      if( current_investment_proportion(best_bid->get_price()) > dip )
	 return false;
      else
      {
	 force_passive = true;
	 return indeterminate;
      }
   }
   else if(best_ask != 0 && best_bid == 0)
   {
      if( current_investment_proportion(best_ask->get_price()) < dip )
	 return true;
      else
      {
	 force_passive = true;
	 return indeterminate;
      }
   }
   else
      return indeterminate;
}

tribool TradingAgent::order_is_bid() const
{
   double dip = desired_investment_proportion();

   tribool preliminary_status = illiquid_market_order_status(dip);

   if(!indeterminate(preliminary_status))
      return preliminary_status;
   else
   {
      double cip_a = current_investment_proportion( spot_mkt->mark_to_market(true) );
      double cip_b = current_investment_proportion( spot_mkt->mark_to_market(false) );

      if(pos.is_long)
      {
	 if(dip > cip_a) return true;
	 else if(dip < cip_b) return false;
	 else return indeterminate;
      }
      else if(!pos.is_long)
      {
	 if(dip > cip_b) return true;
	 else if(dip < cip_a) return false;
	 else return indeterminate;
      }
      else
      {
	 if(dip > 0) return true;
	 else if(dip < 0) return false;
	 else return indeterminate;
      }
   }
}

Position TradingAgent::get_order_quantity(double price) const
{
   double q = pos;
   double omega = desired_investment_proportion();
   double dq = (omega/price)*(q*price + wealth) - q;

   LOG(TRACE,boost::format("Agent %d: with cip= %.4f, dip=%.4f, W=%.0f, Q=%.0f, "
	    "m=%.3f, the requisite raw order quantity is found to be %f\n")
	 % id % current_investment_proportion(price) % desired_investment_proportion() 
	 % wealth % (double) pos % price % dq
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
   force_passive = false;

   tribool order_status = order_is_bid();

   tribool prev_order_status;

   if( !indeterminate(order_status) )
   {
      reset_orders();

      Position dq;

      bool bid = (bool) order_status;

      if( is_active() )
      {
	 double mtm = spot_mkt->mark_to_market( bid );

	 dq = get_order_quantity(mtm);

	 LOG(TRACE,boost::format("Agent %d: Preparing to execute an active order "\
		  "for %.0f units\n")
	       % id % dq
	       );

	 unsigned long ctr = 0;

	 bool stop = false;

	 const Order* best = spot_mkt->best_order( bid );

	 if(best == 0)
	    LOG(TRACE, boost::format(
		     "Agent %d: with order flag (%d) there is "\
		     "no best opposing order\n")
		  % id % bid
		  );
	 else
	    LOG(TRACE, boost::format(
		     "Agent %d: with order flag (%d) the best opposing"
		     "price is %.2f\n")
		  % id % bid % ( best->get_price() )
		  );

	 while (best != 0 && dq != 0 && !stop)
	 {
	    Order act(
		  *this, best->get_price(), dq.q, (bool) dq.is_long,
		  *(*timer)() );
	    
	    LOG(TRACE, boost::format(
		     "Agent %d: beginning pass %d of order-matching sequence "\
		     "for order (%d) of %d units @ %.3f\n")
		  % id % ctr++ % act.is_bid() % act.get_quantity() % act.get_price()
		  );

	    spot_mkt->process_order(act);

	    prev_order_status = order_status;

	    order_status = order_is_bid();

	    if( !indeterminate(order_status) && order_status == !prev_order_status )
	       order_status = indeterminate;

	    if(!indeterminate(order_status))
	    {
	       best = spot_mkt->best_order( (bool) order_status );

	       // if there are any opposing limit orders remaining, continue
	       if(best != 0)
	       {
		  dq = get_order_quantity( best->get_price() );
		  if( (bool) order_status == !act.is_bid() && (dq.q != 0))
		     LOG(EXCEPTION, boost::format("Agent %d: order direction has been "\
			      "reversed in the middle of executing an active order, "\
			      "most recent order parameters: (%d) %d @ %.3f\n")
			   % id % dq.is_long % dq.q % best->get_price()
			   );
		  //there is no way this statement should be here. this must be a mistake
		  //stop = true;
	       }
	       else
	       {
		  if(act.get_quantity() != 0)
		  {
		     LOG(TRACE, boost::format("Agent %d: active order has not been fully "\
			      "executed, sending the remainder to the market: "\
			      "(%d) %.0f @ %.3f\n")
			   % id % act.is_bid() % act.get_quantity() % act.get_price()
			   );
		     spot_mkt->process_order(act);
		  }
		  stop = true;
	       }
	    }
	    else stop = true;
	 }
      }
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
		  *(*timer)() );

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
   check_liquidity();
   check_leverage();
   return !(is_bankrupt || is_overleveraged);
}

void TradingAgent::check_liquidity()
{
   if(!indeterminate(pos.is_long))
   {
      LOG(TRACE, boost::format("Agent %d: Beginning the liquidity check\n")
	    % id
	    );

      double cash_req = spot_mkt->cash_requirement(
				    wealth+investment_value( mark_to_market() )
				    );

      LOG(TRACE, boost::format("Agent %d: with W=%.0f, Q=%.0f, m=%.3f the market "\
	       "cash requirement is %.0f\n")
	    % id % wealth % (double) pos % mark_to_market() % cash_req
	 );

      if(cash_req>0)
      {
	 LOG(TRACE, boost::format("Agent %d: a cash injection of size %.2f "\
		  "is needed in order to stay solvent\n")
	       % id % cash_req
	       );
	 unsigned long rq(0);

	 if(pos.is_long)
	    rq = spot_mkt->raise_cash(cash_req);
	 else
	    rq = spot_mkt->raise_cash(min(cash_req,wealth));

	 if(rq>0)
	 {
	    Order r(*this,rq,(bool)!pos.is_long,*(*timer)());
	    spot_mkt->process_order(r);
	 }

	 is_bankrupt = (spot_mkt->cash_requirement(
				    wealth+investment_value( mark_to_market() )
				    ) > 0);
	 LOG(TRACE, 
	       boost::format("Agent %d: the bankrupcy flag value is now %d\n")
	       % id % is_bankrupt
	       );
      }

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
	    Order close_long(*this, spot_mkt->gauge_market_depth(true,pos.q),false,*(*timer)());
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
      iv = wealth;
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

   tmp("desired_investment_proportion") =
      desired_investment_proportion();

   tmp("current_investment_proportion") = cip;

   tmp("is_bankrupt") = is_bankrupt;

   tmp("is_overleveraged") = is_overleveraged;
   
   return tmp;
}
