// =====================================================================================
// 
//       Filename:  BasicOccasionalInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/09/10 13:47:50
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/BasicOccasionalInfoGenerator.class.h"

using GLOOPER_TEST::BasicOccasionalInfoGenerator;

BasicOccasionalInfoGenerator::BasicOccasionalInfoGenerator(
      double generation_prob): generation_prob(generation_prob)
{}

const bool BasicOccasionalInfoGenerator::generate_now()
{
   return (U() <= generation_prob);
}

XmlField BasicOccasionalInfoGenerator::xml_description() const
{
   XmlField tmp = GLOOPER_TEST::InfoGenerator::xml_description();

   tmp("info_generation_probability") = generation_prob;

   return tmp;
}
