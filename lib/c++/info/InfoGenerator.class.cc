// =====================================================================================
// 
//       Filename:  InfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17/03/10 11:43:03
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/InfoGenerator.class.h"

using GLOOPER_TEST::InfoGenerator;

unsigned long InfoGenerator::instance_ctr = 0ul;

InfoGenerator::InfoGenerator(
      const bspTRGd& value_generator): 
   SimulationObject(instance_ctr++),
   value_generator(value_generator)
{}

double InfoGenerator::get_info(unsigned long turn_timer)
{
   double xi = -1.;

   if( generate_now(turn_timer) )
   {
      xi = (*value_generator)();
      actual_values.push_back(xi);
   }

   info_values.push_back(xi);

   return xi;
}

void InfoGenerator::reset()
{
   info_values.clear();
}

XmlField InfoGenerator::xml_description() const
{
   XmlField tmp("Information.Generator");

   tmp("id") = id;

   tmp("type") = info_generator_type();

   XmlField vg("Value.Generator");
   vg.add_field( value_generator->xml_description() );

   tmp.add_field(vg);

   return tmp;
}
