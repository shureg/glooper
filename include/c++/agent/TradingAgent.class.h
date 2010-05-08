// =====================================================================================
// 
//       Filename:  ClassicAgent.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:53:06
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  _GLT_TRADINGAGENT_CLASS_INC
#define  _GLT_TRADINGAGENT_CLASS_INC

#include "agent/Agent.class.h"
#include "rng/generator/UniformGenerator.class.h"
#include "trading/Position.struct.h"

#include <list>

namespace GLOOPER_TEST
{
   class TradingAgent: public Agent
   {
   public:

      TradingAgent(double,
	    double);

      void place_order();

      XmlField xml_description() const;

      void position_update(const Order&);

      bool can_trade();

   protected:

      Position pos;

      double wealth;

      double desired_investment_proportion() const;

      double current_investment_proportion(double) const;

      double investment_value(double) const;

      bool is_active() const;

      double bid_ask_spread(double,bool) const;

      virtual double spread_fraction() const = 0;

      double mark_to_market() const;

      Position get_order_quantity(double) const;

      void check_liquidity();

      void reset_orders();

      boost::logic::tribool order_is_bid() const;

      bool is_bankrupt;

      mutable bool force_passive;

   private:

      boost::logic::tribool illiquid_market_order_status(double dip) const;

   };
}

#endif   // ----- #ifndef _GLT_TRADINGAGENT_CLASS_INC  -----
