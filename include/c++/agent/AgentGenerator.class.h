// =====================================================================================
// 
//       Filename:  AgentGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/10/09 16:35:33
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  AGENTGENERATOR_CLASS_INC
#define  AGENTGENERATOR_CLASS_INC

#include "core/SimulationObject.class.h"
#include "rng/generator/TypedRandomGenerator.class.hpp"
#include "boost/shared_ptr.hpp"
#include "agent/Agent.class.h"

typedef boost::shared_ptr< RNG::TypedRandomGenerator<double> > bsp_TRGd;
typedef boost::shared_ptr< GLOOPER_TEST::timer_signal > bsp_ts;

namespace GLOOPER_TEST
{
   class AgentGenerator: public SimulationObject
   {
   public:

      AgentGenerator(const bsp_TRGd&, const bsp_TRGd&);

      XmlField xml_description() const;

      virtual Agent* generate_item() = 0;

      virtual const bool stop_generation() = 0;

      void set_timer(const bsp_ts&);

      void set_ro_timer(const bsp_ts&);


   protected:

      boost::shared_ptr< RNG::TypedRandomGenerator<double> > belief_generator;

      boost::shared_ptr< RNG::TypedRandomGenerator<double> > wealth_generator;

      boost::shared_ptr< timer_signal > timer;

      boost::shared_ptr< timer_signal > ro_timer; 

   };
}

#endif   // ----- #ifndef AGENTGENERATOR_CLASS_INC  -----
