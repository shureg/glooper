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

const boost::logic::tribool 
   GLOOPER_TEST::neither = boost::logic::indeterminate;

ComplexAgent::ComplexAgent(double belief,
      double wealth,
      int mean_reversion,
      unsigned long max_memory,
      unsigned long significance_threshold): 
   TradingAgent(belief,wealth),
   distr_byvalue(return_distribution.get<0>()),
   distr_bytime(return_distribution.get<1>()),
   mean_reverter( 
	 (mean_reversion==0)?(neither):(boost::logic::tribool(mean_reversion>0)) ),
   max_memory(max_memory),
   significance_threshold(significance_threshold)
{
   if( significance_threshold > max_memory )
      LOG(EXCEPTION,boost::format(
	       "Complex Agent %d: "\
	       "The significance threshold %d exceeds the maximum "\
	       "memory length %d - belief adjustment will never be invoked\n")
	    % id % significance_threshold % max_memory
	 );
}

void ComplexAgent::init()
{
   market_broadcast_conn = spot_mkt->get_trade_broadcast().connect(
	 boost::bind( &ComplexAgent::add_return,this,_1 ) );
}

ComplexAgent::~ComplexAgent()
{
   market_broadcast_conn.disconnect();
}

const bool ComplexAgent::history_significant() const
{
   return (return_distribution.size() >= significance_threshold);
}

void ComplexAgent::add_return(const Trade& trd)
{
   double last_price;
   if ( !return_distribution.empty() )
      last_price = distr_bytime.back().p1;
   else
      last_price = spot_mkt->last_traded_price();

   distr_bytime.push_back( 
	 price_change( last_price, trd.p ) );
   
   if( max_memory > 0 && distr_bytime.size() > max_memory )
      distr_bytime.pop_front();

   mean_reverter = next_mode();

   if(!indeterminate(mean_reverter) && belief != 0.5)
   {
      double last_ratio = distr_bytime.back().ratio;

      if( history_significant() && last_ratio != 1.)
	 adjust_belief( 
	       (last_ratio > 1.) ? (1. - ecdf(last_ratio)) : (ecdf(last_ratio))
	       );
   }
}

double ComplexAgent::ecdf(double ratio) const
{
   multiset_index::iterator lb = distr_byvalue.lower_bound(ratio);

   return (double) ( distance(distr_byvalue.begin(),lb) * 1. ) / 
      (distr_byvalue.size() * 1.); 
}

XmlField ComplexAgent::xml_description() const
{
   XmlField tmp = TradingAgent::xml_description();

   tmp("max_memory") = max_memory;

   tmp("significance_threshold") = significance_threshold;

   tmp("trend_mode") = 
      (indeterminate(mean_reverter))?("NA"):(
	    ((bool) mean_reverter)?("MR"):("TF"));

   return tmp;
}
