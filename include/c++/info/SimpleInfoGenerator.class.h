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

#include "info/InfoGenerator.class.h"

namespace GLOOPER_TEST
{
   class SimpleInfoGenerator: public InfoGenerator
   {
   public:

      SimpleInfoGenerator(
	    const bspTRGd& value_generator);

   protected:

      const bool generate_now(unsigned long){return true;}

      const char* info_generator_type() const {return "SimpleInfoGenerator";}
   };
}

#endif   // ----- #ifndef SIMPLEINFOGENERATOR_CLASS_INC  -----