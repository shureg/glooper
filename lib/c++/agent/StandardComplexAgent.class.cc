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
      const TRG_d& private_info_generator,
      double wealth,
      const TRG_d& spread_generator,
      int mean_reversion,
      unsigned long max_memory,
      unsigned long significance_threshold,
      double public_weight, double private_weight):
   ComplexAgent(belief, private_info_generator, 
	 wealth, spread_generator, mean_reversion, max_memory, significance_threshold),
   public_proportion( public_weight/(public_weight+private_weight) ), private_proportion(1-public_proportion)
{
}

StandardComplexAgent::~StandardComplexAgent()
{
}

double StandardComplexAgent::process_public_information(double xi) const
{
   return belief + (xi-belief)*U();
}

double StandardComplexAgent::combine_public_private_information(double public_xi, double private_xi) const
{
   return public_proportion*public_xi + private_proportion*private_xi;
}

void StandardComplexAgent::adjust_belief(double p_more_extreme)
{
   bool last_move_up = (last_price_change.ratio > 1.);

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

   belief += (1-p_more_extreme)*(K-belief)*U(); 
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

   tmp("public_proportion") = public_proportion;

   tmp("private_proportion") = private_proportion;

   return tmp;
}
