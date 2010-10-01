// =====================================================================================
// 
//       Filename:  VariableRNGInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 12:33:00
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  VARIABLERNGINFOGENERATOR_CLASS_INC
#define  VARIABLERNGINFOGENERATOR_CLASS_INC

#include "info/RNGInfoGenerator.class.h"

namespace GLOOPER_TEST
{
   class VariableRNGInfoGenerator: public RNGInfoGenerator
   {
   public:

      VariableRNGInfoGenerator(){}

      VariableRNGInfoGenerator(const bspTRGd& value_generator);

   protected:

      void generation_config();

      virtual bspTRGd next_generator() = 0;
   };
}

#endif   // ----- #ifndef VARIABLERNGINFOGENERATOR_CLASS_INC  -----
