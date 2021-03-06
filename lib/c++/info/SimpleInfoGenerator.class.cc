// =====================================================================================
// 
//       Filename:  SimpleInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17/03/10 13:13:25
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/SimpleInfoGenerator.class.h"

using namespace GLOOPER_TEST;

SimpleInfoGenerator::SimpleInfoGenerator(
      const bspTRGd& value_generator):
   RNGInfoGenerator(value_generator)
{}

InfoGenerator* SimpleInfoGenerator::real_clone() const
{
   SimpleInfoGenerator* tmp = new SimpleInfoGenerator(*this);

   return (InfoGenerator*) tmp;
}
