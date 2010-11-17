// =====================================================================================
// 
//       Filename:  TradingAgent.class.h
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

typedef RNG::TypedRandomGenerator<double> TRG_d;
typedef boost::shared_ptr<TRG_d> bspTRG_d;

namespace GLOOPER_TEST
{
   class TradingAgent: public Agent
   {
   public:

      TradingAgent(double, const TRG_d&,
	    double, const TRG_d&);

      void place_order();

      XmlField xml_description() const;

      void position_update(const Order&);

      bool can_trade();

   protected:

      Position pos;

      double wealth;

      const bspTRG_d spread_generator;

      double desired_investment_proportion() const;

      double current_investment_proportion(double) const;

      double investment_value(double) const;

      bool is_active() const;

      double bid_ask_spread(double,bool) const;

      double spread_fraction() const;

      double mark_to_market() const;

      Position get_order_quantity(double) const;

      void check_wealth();

      void check_liquidity();

      void check_leverage();

      void reset_orders();

      boost::logic::tribool order_is_bid() const;

      bool negative_wealth;

      bool is_bankrupt;

      bool is_overleveraged;

      mutable bool force_passive;

      void determine_overleverage(double iv, double L);

   private:

      boost::logic::tribool illiquid_market_order_status(double dip) const;

      const std::string spread_generator_string;
   };
}

#endif   // ----- #ifndef _GLT_TRADINGAGENT_CLASS_INC  -----
