// =====================================================================================
// 
//       Filename:  VariableIntermittentInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 14:13:10
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/VariableIntermittentInfoGenerator.class.h"

using namespace GLOOPER_TEST;

VariableIntermittentInfoGenerator::VariableIntermittentInfoGenerator(
      double generation_prob): BasicOccasionalInfoGenerator(generation_prob)
{}

VariableIntermittentInfoGenerator::VariableIntermittentInfoGenerator(
      double generation_prob,
      const bspTRGd& value_generator): 
   BasicOccasionalInfoGenerator(generation_prob),
   VariableRNGInfoGenerator(value_generator)
{}

XmlField VariableIntermittentInfoGenerator::xml_description() const
{
   XmlField tmp0 = VariableRNGInfoGenerator::xml_description();
   XmlField tmp1 = BasicOccasionalInfoGenerator::xml_description();

   tmp0 += tmp1;

   return tmp0;
}
