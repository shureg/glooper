// =====================================================================================
// 
//       Filename:  RNGInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/09/10 14:56:21
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/RNGInfoGenerator.class.h"

using GLOOPER_TEST::RNGInfoGenerator;

RNGInfoGenerator::RNGInfoGenerator(
      const bspTRGd& value_generator): value_generator(value_generator)
{}

double RNGInfoGenerator::value() const
{
   return (*value_generator)();
}

XmlField RNGInfoGenerator::xml_description() const
{
   XmlField tmp = GLOOPER_TEST::InfoGenerator::xml_description();

   XmlField vg("Value.Generator");
   vg.add_field( value_generator->xml_description() );

   tmp.add_field(vg);

   return tmp;
}
