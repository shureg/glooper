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

using GLOOPER_TEST::SimpleInfoGenerator;

SimpleInfoGenerator::SimpleInfoGenerator(
      const bspTRGd& value_generator):
   InfoGenerator(value_generator)
{}
