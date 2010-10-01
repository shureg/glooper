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

InfoGenerator::InfoGenerator(): 
   SimulationObject(instance_ctr++),
   current_turn_timer(0ul)
{}

double InfoGenerator::get_info(unsigned long turn_timer)
{
   double xi = -1.;

   current_turn_timer = turn_timer;

   generation_config();

   if( generate_now() )
   {
      xi = value();
      actual_values.push_back(xi);
   }

   info_values.push_back(xi);

   return xi;
}

void InfoGenerator::reset()
{
   info_values.clear();
   actual_values.clear();
}

InfoGenerator* InfoGenerator::clone() const
{
   return real_clone();
}

XmlField InfoGenerator::xml_description() const
{
   XmlField tmp("Information.Generator");

   tmp("id") = id;

   tmp("type") = info_generator_type();

   return tmp;
}
