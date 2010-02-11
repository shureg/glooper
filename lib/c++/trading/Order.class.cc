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

unsigned long Order::instance_ctr(0ul);

void Order::reset_instance_ctr()
{
   Order::instance_ctr = 0ul;
}

Order::Order(Agent& _owner,
      double _price, unsigned long _quantity, bool _bid,
      unsigned long _order_time):
   SimulationObject(++instance_ctr),
   owner(_owner),
   price(_price), quantity(_quantity), bid(_bid),
   market(false), order_time(_order_time),
   owner_signal(new trade_signal),
   trade_registration_signal(new detailed_trade_signal)
{
   owner_connection = owner_signal->connect(
	 boost::bind( &Agent::position_update, &owner ,_1 )
	 );

   trade_registration_connection = trade_registration_signal->connect(
	 boost::bind( &Order::record_trade, this, _1 )
	 );

   if( quantity==0 ) LOG(EXCEPTION, 
	 boost::format("Limit order %d (%d) @ %.2f- zero quantity requested\n") 
	 % id % (int) bid % price );
}

Order::Order(Agent& _owner,
      unsigned long _quantity, bool _bid,
      unsigned long _order_time):
   SimulationObject(++instance_ctr),
   owner(_owner),
   price(-1.), quantity(_quantity), bid(_bid),
   market(true), order_time(_order_time),
   owner_signal(new trade_signal),
   trade_registration_signal(new detailed_trade_signal)
{
   owner_connection = owner_signal->connect(
	 boost::bind( &Agent::position_update,&owner,_1 )
	 );

   trade_registration_connection = trade_registration_signal->connect(
	 boost::bind( &Order::record_trade, this, _1 )
	 );
}

Order::~Order()
{
   owner_connection.disconnect();
   trade_registration_connection.disconnect();
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
      Trade tmp(mutual_quantity, r.get_price(),
	    get_id(), r.get_id());
      (*trade_registration_signal)(tmp);
   }
}

void Order::record_trade(const Trade& tr) const
{
   LOG(TRACE, boost::format("Preparing to record a trade for %d units @ %.2f\n"
	    ) % tr.q % tr.p
	 );
   SimulationObject::db_signal()(tr);
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

Agent* Order::get_owner() const
{
   return &owner;
}

bool Order::is_owner(const Agent& _owner) const
{
   return &_owner == &owner;
}


trade_signal& Order::get_owner_signal() const
{
   return *owner_signal;
}

detailed_trade_signal& Order::get_trade_registration_signal() const
{
   return *trade_registration_signal;
}

void Order::check_bid() const
{}

void Order::check_ask() const
{}

XmlField Order::xml_description() const
{
   XmlField tmp("Order");
   tmp("id") = id;
   tmp("price") = price;
   tmp("quantity") = quantity;
   tmp("is_bid") = bid;
   tmp("is_market") = market;
   tmp("order_time") = order_time;
   tmp("owner_id") = owner.get_id();

   return tmp;
}
