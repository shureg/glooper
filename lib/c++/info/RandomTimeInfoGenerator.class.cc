// =====================================================================================
// 
//       Filename:  RandomTimeInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/06/10 13:30:34
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/RandomTimeInfoGenerator.class.h"

using GLOOPER_TEST::RandomTimeInfoGenerator;
using GLOOPER_TEST::InfoGenerator;

RandomTimeInfoGenerator::RandomTimeInfoGenerator(
      const bspTRGd& value_generator,
      const bspTRGd& event_generator):
   InfoGenerator(value_generator),
   event_generator(event_generator)
{}

const bool RandomTimeInfoGenerator::generate_now(unsigned long turn_timer)
{
   return ( U() <= (*event_generator)() );
}

InfoGenerator* RandomTimeInfoGenerator::real_clone() const
{
   RandomTimeInfoGenerator* tmp = new RandomTimeInfoGenerator(*this);

   return (InfoGenerator*) tmp;
}
