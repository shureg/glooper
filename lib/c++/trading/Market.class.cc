// =====================================================================================
// 
//       Filename:  Market.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09/06/09 15:08:15
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "trading/Market.class.h"
#include "boost/bind.hpp"
#include "agent/Agent.class.h"

using namespace GLOOPER_TEST;
using namespace std;

Market::Market(): SimulationObject(0), init_price(100.), minimum_tick(0.01),
   ord_sig(new order_reg_signal)
{}

const bool Market::is_crossing_limit_order(const Order& r) const
{
   if( r.is_market() ) return false;
   else
   {
      if(r.is_bid())
	 return ( !ask_orders.empty() and 
	       r.is_acceptable( *ask_orders.begin() ) );
      else
	 return ( !bid_orders.empty() and 
	       r.is_acceptable( *bid_orders.begin() ) );
   }
}

void Market::process_order(Order& r)
{
   //register incoming order
   SimulationObject::db_signal()(r);

   // A trade may occur
   if( r.is_market() xor is_crossing_limit_order(r) )
   {
      // Connecting trade-recording function slot to the potential
      // trade-emitting signal
      boost::signals::connection tr = 
	 r.get_trade_registration_signal().connect(
	    boost::bind(
	       &Market::record_trade,this,_1) 
	    );

      if(r.is_bid())
      {
	 LOG(TRACE,boost::format("Market: beginning to match a bid order "\
		  "for %d @ %.3f\n")
	       % r.get_quantity() % r.get_price()
	       );

	 set<Order,buyers_pick>::iterator
	    i = ask_orders.begin();
	 
	 while( !r.is_cleared() and i!=ask_orders.end() )
	 {
	    r.match(*i);
	    ++i;
	 }

	 LOG(TRACE,boost::format("Market: removing cleared ask orders\n"));

	 if(!ask_orders.empty())
	 {
	    ask_orders.erase( ask_orders.begin(), --i );
	    if( ask_orders.begin()->is_cleared() ) 
	       ask_orders.erase( ask_orders.begin() );
	 }
      }
      else
      {
	 LOG(TRACE,boost::format("Market: beginning to match an ask order "\
		  "for %d @ %.3f\n")
	       % r.get_quantity() % r.get_price()
	       );

	 set<Order,sellers_pick>::iterator
	    i = bid_orders.begin();
	 
	 while( !r.is_cleared() and i!=bid_orders.end() )
	 {
	    r.match(*i);
	    ++i;
	 }

	 LOG(TRACE,boost::format("Market: removing cleared bid orders\n"));

	 if(!bid_orders.empty())
	 {
	    bid_orders.erase( bid_orders.begin(), --i );
	    if( bid_orders.begin()->is_cleared() ) 
	       bid_orders.erase( bid_orders.begin() );
	 }
      }

      tr.disconnect();

      /*
      if( !r.is_market() && !r.is_cleared() )
      {
	 LOG(TRACE,boost::format("Market: incomplete crossing limit order "\
		  "execution, order still has %d units unmatched\n")
	       % r.get_quantity()
	       );
	 add_limit_order(r,false);
      }
      */
   }
   // A trade will not occur
   else
   {
      add_limit_order(r);
   }

   double p_a(0.), p_b(0.);
   unsigned long q_a(0), q_b(0);

   const Order* r_a = best_order(true);
   const Order* r_b = best_order(false);

   if(r_a != 0)
   {
      p_a = r_a->get_price();
      q_a = r_a->get_quantity();
   }

   if(r_b != 0)
   {
      p_b = r_b->get_price();
      q_b = r_b->get_quantity();
   }

   LOG(TRACE, boost::format("Market: the market quote is now "\
	    "(%d @ %.3f) - (%d @ %.3f)\n")
	 % q_a % p_a % q_b % p_b
	 );

   SimulationObject::db_signal()(*this);
}

void Market::add_limit_order(Order& r, 
      bool left_over)
{
   LOG(TRACE,boost::format(
	    "Market: adding a limit order to the market: %d (%d) @ %f\n")
	 % r.get_quantity() % r.is_bid() % r.get_price() ); 

   if(r.is_bid())
   {
      if(left_over)
	 bid_orders.insert(bid_orders.begin(),r);
      else
	 bid_orders.insert(r);
   }
   else
   {
      if(left_over)
	 ask_orders.insert(ask_orders.begin(),r);
      else
	 ask_orders.insert(r);
   }
}

void Market::pull_agent_orders(const Agent& agt)
{
   set<Order,buyers_pick> new_ask_orders;

   for( set<Order,buyers_pick>::const_iterator
	 i = ask_orders.begin(); i != ask_orders.end(); ++i )
      if( !(i->is_owner(agt)) )
	 new_ask_orders.insert(new_ask_orders.end(), *i );

   ask_orders.clear();
   ask_orders = new_ask_orders;

   set<Order,sellers_pick> new_bid_orders;

   for( set<Order,sellers_pick>::const_iterator
	 i = bid_orders.begin(); i != bid_orders.end(); ++i )
      if( !(i->is_owner(agt)) )
	 new_bid_orders.insert(new_bid_orders.end(), *i );

   bid_orders.clear();
   bid_orders = new_bid_orders;
}

const set<Order,buyers_pick>& Market::get_ask_orders() const
{
   return ask_orders;
}

const set<Order,sellers_pick>& Market::get_bid_orders() const
{
   return bid_orders;
}

const Order* Market::best_order(bool _bid) const
{
   if(_bid)
   {
      if( ask_orders.empty() ) 
	 return 0;
      else
	 return &( *( ask_orders.begin() ) );
   }
   else
   {
      if( bid_orders.empty() ) 
	 return 0;
      else
	 return &( *( bid_orders.begin() ) );
   }
}

void Market::record_trade(const Trade& tr)
{
   trades.push_back(tr);
}

double Market::mark_to_market(bool _bid) const
{
   const Order* tmp = best_order(_bid);
   if( tmp != 0 )
      return tmp->get_price();
   else
   {
      if( !trades.empty() )
	 return trades.back().p;
      else
	 return init_price;
   }
}

double Market::mid_market() const
{
   return 0.5*(mark_to_market(true)+mark_to_market(false));
}

void Market::remove_order(const Order& r)
{
   if(r.is_bid())
      bid_orders.erase(r);
   else
      ask_orders.erase(r);
}

double Market::cash_requirement(const double& cash_position) const
{
   return -min(cash_position,0.);
}

unsigned long Market::raise_cash(const double& _cash) const
{
   bool stop = false;

   double cash = _cash;
   double order_value;
   unsigned long q_m = 0;

   for(set<Order,sellers_pick>::const_iterator
	 i = bid_orders.begin();
	 ( i != bid_orders.end() ) && (!stop);
	 ++i)
   {
      order_value = i->get_price()*(double)(i->get_quantity());
      
      if(order_value <= cash)
      {
	 cash -= order_value;
	 q_m += i->get_quantity();
      }
      else
      {
	 q_m += (unsigned long)(ceil(cash/i->get_price()));
	 stop = true;
      }
   }

   return q_m;
}

double Market::tick_adjusted_price(double p) const
{
   return std::floor(0.5+(p/minimum_tick))*minimum_tick;
}

void Market::reset()
{
   trades.clear();
   bid_orders.clear();
   ask_orders.clear();
}

XmlField Market::xml_description() const
{
   XmlField tmp("Market");
   
   tmp("init_price") = init_price;
   tmp("minimum_tick") = minimum_tick;

   unsigned long depth_ctr;

   if(!ask_orders.empty())
   {
      depth_ctr=0;

      for(set<Order,buyers_pick>::const_iterator
	    i = ask_orders.begin();i!=ask_orders.end();++i)
      {
	 XmlField lo = i->xml_description();
	 lo("depth") = depth_ctr++;
	 tmp.add_field(lo);
      }
   }
   
   if(!bid_orders.empty())
   {
      depth_ctr=0;

      for(set<Order,sellers_pick>::const_iterator
	    i = bid_orders.begin();i!=bid_orders.end();++i)
      {
	 XmlField lo = i->xml_description();
	 lo("depth") = depth_ctr++;
	 tmp.add_field(lo);
      }
   }

   return tmp;
}
