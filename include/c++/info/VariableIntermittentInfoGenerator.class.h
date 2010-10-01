// =====================================================================================
// 
//       Filename:  VariableIntermittentInfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 14:10:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  VARIABLEINTERMITTENTINFOGENERATOR_CLASS_INC
#define  VARIABLEINTERMITTENTINFOGENERATOR_CLASS_INC

#include "VariableRNGInfoGenerator.class.h"
#include "BasicOccasionalInfoGenerator.class.h"

namespace GLOOPER_TEST
{
   class VariableIntermittentInfoGenerator: 
      public BasicOccasionalInfoGenerator, public VariableRNGInfoGenerator
   {
   public:

      VariableIntermittentInfoGenerator(double generation_prob);

      VariableIntermittentInfoGenerator(double generation_prob,
	    const bspTRGd& value_generator);

      XmlField xml_description() const;

   };
}

#endif   // ----- #ifndef VARIABLEINTERMITTENTINFOGENERATOR_CLASS_INC  -----
