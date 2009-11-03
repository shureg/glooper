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

#ifndef  _GLT_CLASSICAGENT_CLASS_INC
#define  _GLT_CLASSICAGENT_CLASS_INC

#include "agent/Agent.class.h"
#include "rng/generator/UniformGenerator.class.h"
#include "trading/Position.struct.h"

#include <list>

namespace GLOOPER_TEST
{
   class ClassicAgent: public Agent
   {
   public:

      ClassicAgent(double,double,double,double,
	    boost::shared_ptr<Market>,double,
	    boost::shared_ptr<timer_signal>,
	    boost::shared_ptr<timer_signal>);

      ~ClassicAgent();

      void place_order();

      XmlField xml_description() const;

      void position_update(const Order&);

      void add_order(Order*);

      bool can_trade();

   protected:

      RNG::UniformGenerator U;

      void update_belief(double);

      bool will_revise(double);

      double revision_proportion(double);

      double gap(double);

      double p_min;

      double f_min;

      double bas_max;

      Position pos;

      double wealth;

      double desired_investment_proportion() const;

      double current_investment_proportion(double) const;

      double investment_value(double) const;

      bool is_active() const;

      double bid_ask_spread(double,bool) const;

      double spread_fraction() const;

      double mark_to_market() const;

      Position get_order_quantity(double) const;

      void check_liquidity();

      std::list<Order*> own_orders;

      void reset_orders();

      boost::logic::tribool order_is_bid() const;

      bool is_bankrupt;

      Agent* real_clone() const;
      
   };
}

#endif   // ----- #ifndef _GLT_CLASSICAGENT_CLASS_INC  -----
