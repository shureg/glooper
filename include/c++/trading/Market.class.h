// =====================================================================================
// 
//       Filename:  Market.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/06/09 16:29:03
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef _GLT_MARKET_CLASS_INC
#define _GLT_MARKET_CLASS_INC

#include "trading/Order.class.h"
#include "trading/Trade.class.h"
#include <set>
#include <list>
#include "core/SimulationObject.class.h"

namespace GLOOPER_TEST
{
   typedef boost::signal< void (Order*) > order_reg_signal;

   class Market: public SimulationObject
   {
   public:

      Market();

      //! Order processing method
      void process_order(Order&);

      const std::set<Order,buyers_pick>&
	 get_ask_orders() const;

      const std::set<Order,sellers_pick>&
	 get_bid_orders() const;

      const Order* best_order(bool) const;

      double mark_to_market(bool) const;

      double mid_market() const;

      void remove_order(const Order&);

      double cash_requirement(const double&) const;

      /*! A method that determines the size of the market order
       * that must be sent to the market to raise a given
       * amount of cash
       */
      unsigned long raise_cash(const double&) const;

      double tick_adjusted_price(double) const;
      
      XmlField xml_description() const;

   protected:

      const bool is_crossing_limit_order(const Order&) const;

      void add_limit_order(Order&,bool=false);

      std::set<Order,buyers_pick> ask_orders;

      std::set<Order,sellers_pick> bid_orders;

      std::list<Trade> trades;

      void record_trade(const Order&);

      double init_price;

      double minimum_tick;

      boost::shared_ptr<order_reg_signal> ord_sig;

   };
}
#endif //_GLT_MARKET_CLASS_INC
