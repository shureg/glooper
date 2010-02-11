// =====================================================================================
// 
//       Filename:  Order.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/06/09 16:32:08
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef _GLT_ORDER_CLASS_INC
#define _GLT_ORDER_CLASS_INC

#include "boost/signals.hpp"
#include "boost/logic/tribool.hpp"
#include "callback_log/LOG.h"
#include <stdexcept>
#include "core/SimulationObject.class.h"
#include "trading/Trade.class.h"

using boost::logic::tribool;

namespace GLOOPER_TEST
{
   class Order;

   typedef boost::signal< void (const Order&) > \
	      trade_signal;

   typedef boost::signal< void (const Trade&) > \
	      detailed_trade_signal;

   class Agent;

   class Order: public SimulationObject
   {
   public:

      //! Limit order constructor
      Order(Agent&,
	    double, unsigned long, bool, unsigned long);

      //! Market order constructor
      Order(Agent&,
	    unsigned long, bool, unsigned long);

      ~Order();

      tribool is_better_for_buyer(const Order&) const;

      tribool is_better_for_seller(const Order&) const;

      const bool is_bid() const;

      const bool is_market() const;

      void match(const Order&) const;

      const double get_price() const;

      const unsigned long get_quantity() const;

      const unsigned long get_order_time() const;

      Agent* get_owner() const;

      bool is_owner(const Agent&) const;

      trade_signal& get_owner_signal() const;

      detailed_trade_signal& get_trade_registration_signal() const;

      void check_bid() const;

      void check_ask() const;

      void execute(unsigned long,double,unsigned long) const;

      const bool is_acceptable(const Order&) const;

      const bool is_cleared() const;

      static void reset_instance_ctr();

      XmlField xml_description() const;

   protected:

      static unsigned long instance_ctr;

      const double price;
      
      mutable unsigned long quantity;
      
      const bool bid;

      const bool market;

      const unsigned long order_time;

      //! A pointer to a trade-processing signal constructed elsewhere
      boost::shared_ptr<trade_signal> owner_signal;

      boost::shared_ptr<detailed_trade_signal> trade_registration_signal;

      boost::signals::connection owner_connection;

      boost::signals::connection trade_registration_connection;

      Agent& owner;

      void record_trade(const Trade&) const;

   private:

      //! Not default-constructible
      Order();

   };

   //! A functor to be used to order offers from best to worst
   struct buyers_pick
   {
      const bool operator() (const Order& r1, const Order& r2) const
      {
	 if( boost::logic::indeterminate( r1.is_better_for_buyer(r2) ) )
	 {
	    if( r1.get_order_time() != r2.get_order_time() )
	       return r1.get_order_time() < r2.get_order_time();
	    else
	       return false;
	 }
	 else
	    return (bool) r1.is_better_for_buyer(r2);
      }
   };

   //! A functor to be used to order bids from best to worst
   struct sellers_pick
   {
      const bool operator() (const Order& r1, const Order& r2) const
      {
	 if( boost::logic::indeterminate( r1.is_better_for_seller(r2) ) )
	 {
	    if( r1.get_order_time() != r2.get_order_time() )
	       return r1.get_order_time() < r2.get_order_time();
	    else
	       return false;
	 }
	 else
	    return r1.is_better_for_seller(r2);
      }
   };

}
#endif //_GLT_ORDER_CLASS_INC
