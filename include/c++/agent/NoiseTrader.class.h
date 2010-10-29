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

      NoiseTrader(double belief, double wealth, 
	    const TRG_d& belief_generator_, double max_bid_ask_sp);

      void update_belief(double);

      XmlField xml_description() const;

      void reconfigure() {}

      void relink() {}

   protected:

      bspTRG_d belief_generator;

      double max_bid_ask_sp;

      RNG::UniformGenerator U;

      double spread_fraction() const;

      const char* agent_type_str() const {return "NoiseTrader";}

      Agent* real_clone() const;

   };
}

#endif   // ----- #ifndef NOISETRADER_CLASS_INC  -----
