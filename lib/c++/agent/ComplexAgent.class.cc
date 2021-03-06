// =====================================================================================
// 
//       Filename:  ComplexAgent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  12/02/10 13:23:22
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/ComplexAgent.class.h"
#include "boost/bind.hpp"
#include "callback_log/LOG.h"

using namespace GLOOPER_TEST;

using namespace std;

const boost::logic::tribool 
   GLOOPER_TEST::neither = boost::logic::indeterminate;

ComplexAgent::ComplexAgent(double belief,
      const TRG_d& private_info_generator,
      double wealth,
      const TRG_d& spread_generator,
      int mean_reversion,
      unsigned long max_memory,
      unsigned long significance_threshold): 
   TradingAgent(belief,private_info_generator,wealth,spread_generator),
   mean_reverter( 
	 (mean_reversion==0)?(neither):(boost::logic::tribool(mean_reversion>0)) ),
   max_memory(max_memory),
   significance_threshold(significance_threshold)
{
   if( max_memory > 0 && significance_threshold > max_memory )
      LOG(EXCEPTION,boost::format(
	       "Complex Agent %d: "\
	       "The significance threshold %d exceeds the maximum "\
	       "memory length %d - belief adjustment will never be invoked\n")
	    % id % significance_threshold % max_memory
	 );
}

void ComplexAgent::relink()
{
   market_broadcast_conn.disconnect();
   market_broadcast_conn = spot_mkt->get_trade_broadcast().connect(
	 boost::bind( &ComplexAgent::add_return,this,_1 ) );
}

ComplexAgent::~ComplexAgent()
{
   market_broadcast_conn.disconnect();
}

const bool ComplexAgent::history_significant(
      const deque<price_change>& distr_bytime) const
{
   return (distr_bytime.size() >= significance_threshold);
}

void ComplexAgent::add_return(const Trade& trd)
{
   LOG(TRACE, boost::format(
	    "Entering ComplexAgent::add_return for agent %d at %0x\n")
	 % id % this
	 );

   double last_price;

   if ( last_price_change.set )
      last_price = last_price_change.p1;
   else
      last_price = spot_mkt->last_traded_price();

   LOG(TRACE, boost::format(
	    "ComplexAgent::add_return - last price determined "\
	    "(agent %d at %0x\n)")
	 % id % this
	 );

   price_change dp(last_price, trd.p);

   LOG(TRACE, boost::format(
	    "ComplexAgent::price changed recorded "\
	    "(agent %d at %0x\n)")
	 % id % this
      );

   double last_ratio = dp.ratio;

   last_price_change = dp;

   if(last_ratio != 1)
   {
      deque<price_change>& distr_bytime =
	 (last_ratio > 1)?(distr_bytime_pos):(distr_bytime_neg);
      
      multiset<double>& distr_byvalue =
	 (last_ratio > 1)?(distr_byvalue_pos):(distr_byvalue_neg);

      dp.where =
	 distr_byvalue.insert( dp.ratio );

      distr_bytime.push_back( 
	    price_change( dp ) );

      if( max_memory > 0 && distr_bytime.size() > max_memory )
      {
	 distr_byvalue.erase(distr_bytime.front().where);
	 distr_bytime.pop_front();
      }

      mean_reverter = next_mode();

      if(!indeterminate(mean_reverter) && belief != 0.5)
      {
	 LOG(TRACE, boost::format(
		  "[TREND]: Agent %d - Last available price ratio determined to be %.4f\n")
	       % id % last_ratio
	    );

	 if( history_significant(distr_bytime) )
	 {
	    double b0 = belief;
	    adjust_belief( 
		  (last_ratio > 1.) ? (1. - ecdf(last_ratio)) : (ecdf(last_ratio))
		  );
	    LOG(TRACE, boost::format(
		     "[TREND]: Agent %d - belief change "\
		     "after adjustment is %.8f\n")
		  % id % (belief - b0)
	       );
	 }
      }

   }
}

double ComplexAgent::ecdf(double ratio) const
{
   const multiset<double>& distr_byvalue =
      (ratio > 1)?(distr_byvalue_pos):(distr_byvalue_neg);

   multiset<double>::iterator 
      lb = distr_byvalue.lower_bound(ratio);

   return (double) ( distance(distr_byvalue.begin(),lb) * 1. ) / 
      (distr_byvalue.size() * 1.); 
}

XmlField ComplexAgent::xml_description() const
{
   XmlField tmp = TradingAgent::xml_description();

   tmp("max_memory") = max_memory;

   tmp("significance_threshold") = significance_threshold;

   //tmp("trend_mode") = 
   //   (indeterminate(mean_reverter))?("NA"):(
	//    ((bool) mean_reverter)?("MR"):("TF"));

   if(indeterminate(mean_reverter))
      tmp("trend_mode") = "NA";
   else if ( (bool) mean_reverter == true )
      tmp("trend_mode") = "MR";
   else
      tmp("trend_mode") = "TF";

   return tmp;
}
