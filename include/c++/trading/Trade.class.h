// =====================================================================================
// 
//       Filename:  Trade.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  29/09/09 19:10:06
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  TRADE_CLASS_INC_INC
#define  TRADE_CLASS_INC_INC

#include"core/SimulationObject.class.h"

namespace GLOOPER_TEST
{
   class Trade: public SimulationObject
   {
   public:

      Trade(unsigned long, double);

      XmlField xml_description() const;

      unsigned long q;

      double p;

   protected:

      static unsigned long instance_ctr;

   private:

      Trade();
   };
}

#endif   // ----- #ifndef TRADE_CLASS_INC_INC  -----
