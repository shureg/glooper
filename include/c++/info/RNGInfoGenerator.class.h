// =====================================================================================
// 
//       Filename:  RNGInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/09/10 13:50:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RNGINFOGENERATOR_CLASS_INC
#define  RNGINFOGENERATOR_CLASS_INC

#include "info/InfoGenerator.class.h"

#include "rng/generator/TypedRandomGenerator.class.hpp"
#include "boost/shared_ptr.hpp"

typedef RNG::TypedRandomGenerator<double> TRGd;
typedef boost::shared_ptr< TRGd > bspTRGd;

namespace GLOOPER_TEST
{
   class RNGInfoGenerator: public virtual InfoGenerator
   {
   public:

      RNGInfoGenerator(){}

      RNGInfoGenerator(const bspTRGd& value_generator);

      XmlField xml_description() const;

   protected:

      mutable bspTRGd value_generator;

      double value() const;

   };
}

#endif   // ----- #ifndef RNGINFOGENERATOR_CLASS_INC  -----
