// =====================================================================================
// 
//       Filename:  ClassicAgent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:57:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/ClassicAgent.class.h"
#include <cmath>
#include <iostream>
#include "rng/generator/UniformGenerator.class.h"
#include "boost/bind.hpp"
#include "callback_log/LOG.h"

using namespace GLOOPER_TEST;
using namespace std;
using namespace boost::logic;

ClassicAgent::ClassicAgent(double belief,
      double wealth,
      double p_min, double f_min, double bas_max):
   TradingAgent(belief,wealth),
   p_min(p_min), f_min(f_min), bas_max(bas_max), 
   spread_gen( RNG::UniformGenerator(bas_max/10.,bas_max) )

{
}

ClassicAgent::~ClassicAgent()
{
}

bool ClassicAgent::will_revise(double new_belief)
{
   double db = abs(new_belief-belief);

   if( db > 0 )
      return ( U() <= ((1-p_min)*db + p_min) );
   else
      return false;
}

double ClassicAgent::revision_proportion(double new_belief)
{
   double g = gap(belief);

   if( belief >= 0.5 )
   {
      if( new_belief >= belief )
	 return 1. + 0.5*(1.-g)*(new_belief-1.)/(1.-belief);
      else
	 return f_min + 0.5*new_belief*(1.-g-2.*f_min)/belief;
   }
   else
   {
      if( new_belief < belief )
	 return 1. + 0.5*(g-1)*new_belief/belief;
      else
	 return 0.5*(1.-g) + 0.5*(2.*f_min+g-1.)*(new_belief-belief)/(1.-belief);
   }
}

double ClassicAgent::gap(double b)
{
   return abs(b-0.5);
}

void ClassicAgent::update_belief(double new_belief)
{
   if( will_revise(new_belief) )
   {
      belief += revision_proportion(new_belief)*(new_belief-belief);
   }
}

double ClassicAgent::spread_fraction() const
{
   return spread_gen();
}


XmlField ClassicAgent::xml_description() const
{
   XmlField tmp = TradingAgent::xml_description();

   tmp("minimum_revision_probability") = p_min;

   tmp("minimum_belief_adjustment_proportion") = f_min;

   tmp("maximum_bid-ask_spread") = bas_max;

   return tmp;
}

Agent* ClassicAgent::real_clone() const
{
   Agent* tmp = (Agent*) new ClassicAgent(*this);

   return tmp;
}

const char* ClassicAgent::agent_type_str() const
{
   return "ClassicAgent";
}
