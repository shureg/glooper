// =====================================================================================
// 
//       Filename:  HomogeneousAgent.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  16/05/10 10:59:54
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  HOMOGENEOUSAGENT_CLASS_INC
#define  HOMOGENEOUSAGENT_CLASS_INC

#include "agent/Agent.class.h"
#include "rng/generator/UniformGenerator.class.h"

namespace GLOOPER_TEST
{
   class HomogeneousAgent: public Agent
   {
   public:

      HomogeneousAgent(
	    double belief, 
	    unsigned long order_size,
	    double min_spread, double max_spread);

      void update_belief(double);

      void place_order();

      void position_update(const Order&){}

      bool can_trade(){return true;}

      void reconfigure(){}

      XmlField xml_description() const;

   protected:

      RNG::UniformGenerator U;

      unsigned long order_size;

      double min_spread, max_spread;

      RNG::UniformGenerator spread_gen;

      const char* agent_type_str() const {return "HomogeneousAgent";}

      Agent* real_clone() const;
      
   };
}

#endif   // ----- #ifndef HOMOGENEOUSAGENT_CLASS_INC  -----
