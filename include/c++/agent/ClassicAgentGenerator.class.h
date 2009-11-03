// =====================================================================================
// 
//       Filename:  ClassicAgentGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/10/09 19:56:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  CLASSICAGENTGENERATOR_CLASS_INC
#define  CLASSICAGENTGENERATOR_CLASS_INC

#include "agent/AgentGenerator.class.h"

using RNG::TypedRandomGenerator;

namespace GLOOPER_TEST
{
   class Market;

   class ClassicAgentGenerator: public AgentGenerator
   {
   public:

      ClassicAgentGenerator(
	    const bsp_TRGd&, const bsp_TRGd&,
	    const bsp_TRGd&, const bsp_TRGd&, const bsp_TRGd&,
	    const boost::shared_ptr<Market>&, unsigned long);

      Agent* generate_item();

      const bool stop_generation();

      XmlField xml_description() const;

   protected:

      boost::shared_ptr< TypedRandomGenerator<double> > f_min_gen;

      boost::shared_ptr< TypedRandomGenerator<double> > p_min_gen;
      
      boost::shared_ptr< TypedRandomGenerator<double> > bas_gen;

      boost::shared_ptr<Market> mkt;

      unsigned long N_agents;

      unsigned long agt_ctr;
   };
}

#endif   // ----- #ifndef CLASSICAGENTGENERATOR_CLASS_INC  -----
