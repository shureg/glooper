// =====================================================================================
// 
//       Filename:  TriangularMarkovianInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 14:18:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  TRIANGULARMARKOVIANINFOGENERATOR_CLASS_INC
#define  TRIANGULARMARKOVIANINFOGENERATOR_CLASS_INC

#include "info/VariableIntermittentInfoGenerator.class.h"

namespace GLOOPER_TEST
{
   class TriangularMarkovianInfoGenerator: 
      public VariableIntermittentInfoGenerator
   {
   public:

      TriangularMarkovianInfoGenerator(
	    double generation_prob);

   protected:

      bspTRGd next_generator();

      const char* info_generator_type() const
      {return "TriangularMarkovianInfoGenerator";}

      InfoGenerator* real_clone() const;
   };
}

#endif   // ----- #ifndef TRIANGULARMARKOVIANINFOGENERATOR_CLASS_INC  -----
