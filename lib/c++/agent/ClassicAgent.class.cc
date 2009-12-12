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

ClassicAgent::ClassicAgent(double _belief, double _p_min, double _f_min,
      double _wealth, boost::shared_ptr<Market> _spot_mkt, double _bas_max,
      boost::shared_ptr<timer_signal> _timer, 
      boost::shared_ptr<timer_signal> _ro_timer):
   Agent(_belief,_spot_mkt,_timer,_ro_timer), p_min(_p_min), f_min(_f_min),
   wealth(_wealth), bas_max(_bas_max), is_bankrupt(false)
{
   LOG(TRACE,
	 boost::format("Agent %d created, preparing to save the initial state\n")
	 % id);

   SimulationObject::db_signal()(*this);
}

ClassicAgent::~ClassicAgent()
{
}

bool ClassicAgent::will_revise(double new_belief)
{
   double db = abs(new_belief-belief);

   if( db > 0 )
      return ( U() <= ((1-p_min)*db + p_min) );
   else
      return false;
}

double ClassicAgent::revision_proportion(double new_belief)
{
   double g = gap(belief);

   if( belief >= 0.5 )
   {
      if( new_belief >= belief )
	 return 1. + 0.5*(1.-g)*(new_belief-1.)/(1.-belief);
      else
	 return f_min + 0.5*new_belief*(1.-g-2.*f_min)/belief;
   }
   else
   {
      if( new_belief < belief )
	 return 1. + 0.5*(g-1)*new_belief/belief;
      else
	 return 0.5*(1.-g) + 0.5*(2.*f_min+g-1.)*(new_belief-belief)/(1.-belief);
   }
}

double ClassicAgent::gap(double b)
{
   return abs(b-0.5);
}

void ClassicAgent::update_belief(double new_belief)
{
   if( will_revise(new_belief) )
   {
      belief += revision_proportion(new_belief)*(new_belief-belief);
   }
}

double ClassicAgent::investment_value(double mtm) const
{
   return ((double) pos)*mtm;
}

double ClassicAgent::current_investment_proportion(double mtm) const
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

double ClassicAgent::desired_investment_proportion() const
{
   return 2.*(belief-0.5);
}

void ClassicAgent::position_update(const Order& r)
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

bool ClassicAgent::is_active() const
{
   return 2.*abs(belief-0.5) > 0.75;
}

double ClassicAgent::bid_ask_spread(double price,bool is_bid) const
{
   if(is_bid)
      return price/(1.+spread_fraction());
   else
      return price*(1.+spread_fraction());
}

tribool ClassicAgent::order_is_bid() const
{
   double dip = desired_investment_proportion();
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

Position ClassicAgent::get_order_quantity(double price) const
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

void ClassicAgent::place_order()
{
   tribool order_status = order_is_bid();

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

	 while (best != 0 && dq != 0 && !stop)
	 {
	    Order act(
		  *this, best->get_price(), dq.q, (bool) dq.is_long,
		  (*timer)() );
	    
	    LOG(TRACE, boost::format(
		     "Agent %d: beginning pass %d of order-matching sequence "\
		     "for order (%d) of %d units @ %.3f\n")
		  % id % ctr++ % act.is_bid() % act.get_quantity() % act.get_price()
		  );

	    spot_mkt->process_order(act);
	    
	    order_status = order_is_bid();

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
		  (*timer)() );

	    spot_mkt->process_order(pass);
	 }
      }
   }

   SimulationObject::db_signal()(*this);
}

void ClassicAgent::reset_orders()
{
   spot_mkt->pull_agent_orders(*this);
   LOG(TRACE, boost::format("Agent %d: own orders have been reset\n") % id);
}

double ClassicAgent::spread_fraction() const
{
   RNG::UniformGenerator U(bas_max/10.,bas_max);

   return U();
}

double ClassicAgent::mark_to_market() const
{
   if(indeterminate(pos.is_long))
      LOG(EXCEPTION, boost::format("Agent %d: the internal mark-to-market method "\
	       "may not be called when there is no position in the asset\n")
	    % id
	    );
   return spot_mkt->mark_to_market(!pos.is_long);
}

bool ClassicAgent::can_trade()
{
   check_liquidity();
   return !is_bankrupt;
}

void ClassicAgent::check_liquidity()
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
	    Order r(*this,rq,(bool)!pos.is_long,(*timer)());
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

XmlField ClassicAgent::xml_description() const
{
   XmlField tmp = Agent::xml_description();

   tmp("type") = "ClassicAgent";
   
   tmp("wealth") = wealth;

   tmp("position") = (double) pos;

   tmp("desired_investment_proportion") =
      desired_investment_proportion();

   tmp("minimum_revision_probability") = p_min;

   tmp("minimum_belief_adjustment_proportion") = f_min;

   tmp("maximum_bid-ask_spread") = bas_max;

   tmp("is_bankrupt") = is_bankrupt;
   
   return tmp;
}

Agent* ClassicAgent::real_clone() const
{
   Agent* tmp = (Agent*) new ClassicAgent(*this);

   return tmp;
}
