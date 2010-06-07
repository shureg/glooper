// =====================================================================================
// 
//       Filename:  RandomTimeInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/06/10 13:26:03
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RANDOMTIMEINFOGENERATOR_CLASS_INC
#define  RANDOMTIMEINFOGENERATOR_CLASS_INC

#include "info/InfoGenerator.class.h"
#include "rng/generator/UniformGenerator.class.h"

namespace GLOOPER_TEST
{
   class RandomTimeInfoGenerator: public InfoGenerator
   {
   public:

      RandomTimeInfoGenerator(
	    const bspTRGd& value_generator,
	    const bspTRGd& event_generator);
   
   protected:

      bspTRGd event_generator;

      RNG::UniformGenerator U;

      const bool generate_now(unsigned long);

      const char* info_generator_type() const {return "RandomTimeInfoGenerator";}

   };
}

#endif   // ----- #ifndef RANDOMTIMEINFOGENERATOR_CLASS_INC  -----
