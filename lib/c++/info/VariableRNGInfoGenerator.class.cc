// =====================================================================================
// 
//       Filename:  VariableRNGInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 13:58:10
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/VariableRNGInfoGenerator.class.h"

using namespace GLOOPER_TEST;

VariableRNGInfoGenerator::VariableRNGInfoGenerator(
      const bspTRGd& value_generator): RNGInfoGenerator(value_generator)
{}

void VariableRNGInfoGenerator::generation_config()
{
   value_generator = next_generator();
}
