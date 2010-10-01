// =====================================================================================
// 
//       Filename:  SimpleInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17/03/10 13:09:30
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  SIMPLEINFOGENERATOR_CLASS_INC
#define  SIMPLEINFOGENERATOR_CLASS_INC

#include "info/RNGInfoGenerator.class.h"

namespace GLOOPER_TEST
{
   class SimpleInfoGenerator: public RNGInfoGenerator
   {
   public:

      SimpleInfoGenerator(
	    const bspTRGd& value_generator);

   protected:

      const bool generate_now(){return true;}

      const char* info_generator_type() const {return "SimpleInfoGenerator";}

      InfoGenerator* real_clone() const;
   };
}

#endif   // ----- #ifndef SIMPLEINFOGENERATOR_CLASS_INC  -----
