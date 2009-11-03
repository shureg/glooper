// =====================================================================================
// 
//       Filename:  Order.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09/06/09 11:11:20
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "trading/Order.class.h"
#include <algorithm>
#include "agent/Agent.class.h"
#include "boost/bind.hpp"

using namespace GLOOPER_TEST;
using boost::logic::tribool;
using boost::logic::indeterminate;
using CALLBACK_LOG::LOG;

unsigned long Order::instance_counter(0ul);

Order::Order(Agent& _owner,
      double _price, unsigned long _quantity, bool _bid,
      unsigned long _order_time):
   SimulationObject(++instance_counter),
   owner(_owner),
   price(_price), quantity(_quantity), bid(_bid),
   market(false), order_time(_order_time),
   owner_signal(new trade_signal)
{
   owner_connection = owner_signal->connect(
	 boost::bind( &Agent::position_update,&owner,_1 )
	 );
}

Order::Order(Agent& _owner,
      unsigned long _quantity, bool _bid,
      unsigned long _order_time):
   SimulationObject(++instance_counter),
   owner(_owner),
   price(-1.), quantity(_quantity), bid(_bid),
   market(true), order_time(_order_time),
   owner_signal(new trade_signal)
{
   owner_connection = owner_signal->connect(
	 boost::bind( &Agent::position_update,&owner,_1 )
	 );
}

Order::~Order()
{
   owner_connection.disconnect();
}

tribool Order::is_better_for_buyer(const Order& r) const
{
   if(price != r.get_price())
      return price < r.get_price();
   else
      return indeterminate;
}

tribool Order::is_better_for_seller(const Order& r) const
{
   if(price != r.get_price())
      return price > r.get_price();
   else
      return indeterminate;
}

void Order::match(const Order& r) const
{
   if( is_acceptable(r) )
   {
      unsigned long mutual_quantity = std::min(quantity,r.get_quantity());
      LOG(TRACE,boost::format("Order (%d) for %d @ %.3f: executing %d @ %.3f\n")
	    % bid % quantity % price % mutual_quantity % r.get_price() 
	    );
      execute(mutual_quantity,r.get_price(),order_time);
      r.execute(mutual_quantity,r.get_price(),order_time);
   }
}

void Order::execute(
      unsigned long executed_quantity, double execution_price,
      unsigned long execution_time) const
{
   quantity -= executed_quantity;
   (*owner_signal)(Order(
	    owner,execution_price,executed_quantity,bid,execution_time));
}

const bool Order::is_acceptable(const Order& r) const
{
   if(bid == !r.is_bid())
   {
      if(market) return true;
      else
      {
	 if(bid) return price >= r.get_price();
	 else return price <= r.get_price();
      }
   }
   else return false;
}

const bool Order::is_bid() const
{
   return bid;
}

const bool Order::is_market() const
{
   return market;
}

const double Order::get_price() const
{
   return price;
}

const unsigned long Order::get_quantity() const
{
   return quantity;
}

const bool Order::is_cleared() const
{
   return quantity == 0;
}

const unsigned long Order::get_order_time() const
{
   return order_time;
}

Agent* Order::get_owner()
{
   return &owner;
}


trade_signal& Order::get_owner_signal()
{
   return *owner_signal;
}

unsigned long Order::get_id() const
{
   return id;
}

void Order::check_bid() const
{}

void Order::check_ask() const
{}

XmlField Order::xml_description() const
{
   XmlField tmp("Order");
   tmp.add_field("id",id);
   tmp.add_field("price",price);
   tmp.add_field("quantity",quantity);
   tmp.add_field("is_bid",bid);
   tmp.add_field("is_market",market);
   tmp.add_field("order_time",order_time);

   return tmp;
}
