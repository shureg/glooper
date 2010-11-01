// =====================================================================================
// 
//       Filename:  StandardComplexAgent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/30/2010 07:45:15 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/StandardComplexAgent.class.h"

#include <iostream>

using namespace GLOOPER_TEST;

StandardComplexAgent::StandardComplexAgent(
      double belief,
      double wealth,
      const TRG_d& spread_generator,
      int mean_reversion,
      unsigned long max_memory,
      unsigned long significance_threshold):
   ComplexAgent(belief, wealth, spread_generator, mean_reversion, max_memory, significance_threshold)
{
}

StandardComplexAgent::~StandardComplexAgent()
{
}

void StandardComplexAgent::update_belief(double xi)
{
   belief += (xi-belief)*U();
}

void StandardComplexAgent::adjust_belief(double p_more_extreme)
{
   bool last_move_up = (distr_bytime.back().ratio > 1.);

   bool mr = (bool) mean_reverter;

   bool codirected = (belief > 0.5 and last_move_up) or (belief < 0.5 and !last_move_up);

   double K;

   if( (mr and codirected) or (!mr && !codirected) )
      K = 0.5;
   else
   {
      if(belief > 0.5)
	 K = 1.;
      else
	 K = 0.;
   }

   belief += (1-p_more_extreme)*(K-belief); 
}

const boost::logic::tribool StandardComplexAgent::next_mode() const
{
   return mean_reverter;
}

Agent* StandardComplexAgent::real_clone() const
{
   StandardComplexAgent* tmp = new StandardComplexAgent(*this);
   return (Agent*) tmp;
}

XmlField StandardComplexAgent::xml_description() const
{
   XmlField tmp = ComplexAgent::xml_description();

   return tmp;
}
