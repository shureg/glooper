// =====================================================================================
// 
//       Filename:  NoiseTrader.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/26/2010 08:40:05 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  NOISETRADER_CLASS_INC
#define  NOISETRADER_CLASS_INC

#include "agent/TradingAgent.class.h"
#include "rng/generator/UniformGenerator.class.h"

namespace GLOOPER_TEST
{
   typedef RNG::TypedRandomGenerator<double> TRG_d;
   typedef boost::shared_ptr< TRG_d > bspTRG_d;

   class NoiseTrader: public TradingAgent
   {
   public:

      NoiseTrader(double belief, double wealth,  const TRG_d& spread_generator,
	    const TRG_d& belief_generator_);

      ~NoiseTrader();

      void update_belief(double);

      XmlField xml_description() const;

      void reconfigure() {}

      void relink() {}

   protected:

      const bspTRG_d belief_generator;

      const char* agent_type_str() const {return "NoiseTrader";}

      Agent* real_clone() const;

   private:

      const std::string belief_generator_string;

   };
}

#endif   // ----- #ifndef NOISETRADER_CLASS_INC  -----
