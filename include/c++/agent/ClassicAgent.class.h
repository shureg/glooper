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

#include "agent/TradingAgent.class.h"
#include "rng/generator/UniformGenerator.class.h"
#include "trading/Position.struct.h"

#include <list>

namespace GLOOPER_TEST
{
   class ClassicAgent: public TradingAgent
   {
   public:

      ClassicAgent(double belief,
	    double wealth,
	    double p_min, double f_min, double bas_max);

      ~ClassicAgent();

      XmlField xml_description() const;

      void update_belief(double);

      void init(){}

   protected:

      RNG::UniformGenerator U;

      bool will_revise(double);

      double revision_proportion(double);

      double gap(double);

      double p_min;

      double f_min;

      double bas_max;

      RNG::UniformGenerator spread_gen;

      double spread_fraction() const;

      Agent* real_clone() const;

      const char* agent_type_str() const;

   };
}

#endif   // ----- #ifndef _GLT_CLASSICAGENT_CLASS_INC  -----
