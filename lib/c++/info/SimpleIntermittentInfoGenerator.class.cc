// =====================================================================================
// 
//       Filename:  SimpleIntermittentInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 11:51:29
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/SimpleIntermittentInfoGenerator.class.h"

using namespace GLOOPER_TEST;

SimpleIntermittentInfoGenerator::SimpleIntermittentInfoGenerator(
      const bspTRGd& value_generator, double generation_prob):
   RNGInfoGenerator(value_generator),
   BasicOccasionalInfoGenerator(generation_prob)
{}

InfoGenerator* SimpleIntermittentInfoGenerator::real_clone() const
{
   SimpleIntermittentInfoGenerator* tmp =
      new SimpleIntermittentInfoGenerator(*this);

   return (InfoGenerator*) tmp;
}

XmlField SimpleIntermittentInfoGenerator::xml_description() const
{
   XmlField tmp0 = RNGInfoGenerator::xml_description();
   XmlField tmp1 = BasicOccasionalInfoGenerator::xml_description();

   tmp0 += tmp1;

   return tmp0;
}
