// =====================================================================================
// 
//       Filename:  BasicOccasionalInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/07/10 22:05:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef BASIC_OCCASIONAL_INFO_GENERATOR
#define BASIC_OCCASIONAL_INFO_GENERATOR

#include "info/InfoGenerator.class.h"

#include "rng/generator/UniformGenerator.class.h"

namespace GLOOPER_TEST
{
   class BasicOccasionalInfoGenerator: public virtual InfoGenerator
   {
   public:

      BasicOccasionalInfoGenerator();

      BasicOccasionalInfoGenerator(double generation_prob);

      XmlField xml_description() const;

   protected:

      double generation_prob;

      RNG::UniformGenerator U;

      const bool generate_now();

   };
}

#endif // -------- #ifndef BASIC_OCCASIONAL_INFO_GENERATOR ---------
