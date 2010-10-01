// =====================================================================================
// 
//       Filename:  SimpleIntermittentInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/09/10 17:01:07
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  SIMPLEINTERMITTENTINGOGENERATOR_CLASS_INC
#define  SIMPLEINTERMITTENTINGOGENERATOR_CLASS_INC

#include "BasicOccasionalInfoGenerator.class.h"
#include "RNGInfoGenerator.class.h"

namespace GLOOPER_TEST
{
   class SimpleIntermittentInfoGenerator:
      public RNGInfoGenerator, public BasicOccasionalInfoGenerator
   {
   public:

      SimpleIntermittentInfoGenerator(
	    const bspTRGd& value_generator, 
	    double generation_prob);

      XmlField xml_description() const;

   protected:

      const char* info_generator_type() const
      {return "SimpleIntermittentInfoGenerator";}

      InfoGenerator* real_clone() const;
   };
}

#endif   // ----- #ifndef SIMPLEINTERMITTENTINGOGENERATOR_CLASS_INC  -----
