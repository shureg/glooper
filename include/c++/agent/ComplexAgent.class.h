// =====================================================================================
// 
//       Filename:  ComplexAgent.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  12/02/10 13:17:23
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  COMPLEXAGENT_CLASS_INC
#define  COMPLEXAGENT_CLASS_INC

#include "agent/TradingAgent.class.h"
#include "trading/Trade.class.h"

#include <utility>
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/logic/tribool.hpp"
#include <cmath>

#include "boost/signals2.hpp"

#include <set>
#include <deque>

namespace GLOOPER_TEST
{
   struct price_change
   {
      double p0,p1;
      double ratio;
      std::multiset<double>::iterator where;

      price_change(double _p0, double _p1):
	 p0(_p0),p1(_p1),ratio(p1/p0)
      {}
   };

   using namespace boost;
   //using namespace boost::multi_index;

   /*typedef multi_index_container< price_change,
	   indexed_by< 
	      ordered_non_unique<
	      member<
		  price_change,double,&price_change::ratio>
		  >,
	      sequenced<> >
	    > price_history;

   typedef price_history::nth_index<0>::type multiset_index;
   typedef price_history::nth_index<1>::type list_index;*/

   extern const boost::logic::tribool neither;

   class ComplexAgent: public TradingAgent
   {
   public:

      ComplexAgent(double belief,
	    double wealth,
	    int mean_reversion,
	    unsigned long max_memory,
	    unsigned long significance_threshold);

      ~ComplexAgent();

      XmlField xml_description() const;

      void reconfigure();

   protected:

      //price_history return_distribution;

      //multiset_index& distr_byvalue;
      //list_index& distr_bytime;
      
      std::multiset<double> distr_byvalue;
      std::deque<price_change> distr_bytime;

      boost::logic::tribool mean_reverter;

      unsigned long max_memory;

      unsigned long significance_threshold;

      void add_return(const Trade& trd);

      //! Empirical distribution function of historical price ratios
      /*! Returns the empirical probability that a price ratio is
       *  strictly smaller than a given value 
       */
      double ecdf(double ratio) const;

      virtual const bool history_significant() const;

      virtual void adjust_belief(double p_more_extreme) = 0;

      virtual const boost::logic::tribool next_mode() const = 0;

   private:

      boost::signals2::connection market_broadcast_conn;
   };
}

#endif   // ----- #ifndef COMPLEXAGENT_CLASS_INC  -----
