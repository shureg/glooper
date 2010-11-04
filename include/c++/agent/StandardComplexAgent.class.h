// =====================================================================================
// 
//       Filename:  StandardComplexAgent.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/30/2010 07:38:26 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  STANDARDCOMPLEXAGENT_CLASS_INC
#define  STANDARDCOMPLEXAGENT_CLASS_INC

#include "agent/ComplexAgent.class.h"

#include "rng/generator/UniformGenerator.class.h"

namespace GLOOPER_TEST
{
   class StandardComplexAgent: public ComplexAgent
   {
   public:

      StandardComplexAgent(double belief,
	    const TRG_d& private_info_generator,
	    double wealth,
	    const TRG_d& spread_generator,
	    int mean_reversion,
	    unsigned long max_memory,
	    unsigned long significance_threshold,
	    double public_weight,
	    double private_weight);

      ~StandardComplexAgent();

      XmlField xml_description() const;

      void reconfigure(){}

   protected:

      double public_proportion;

      double private_proportion;

      RNG::UniformGenerator U;

      double process_public_information(double) const;

      double combine_public_private_information(double, double) const;

      void adjust_belief(double p_more_extreme);

      const boost::logic::tribool next_mode() const;

      const char* agent_type_str() const {return "StandardComplexAgent";}

      Agent* real_clone() const;
   };
}

#endif   // ----- #ifndef STANDARDCOMPLEXAGENT_CLASS_INC  -----
