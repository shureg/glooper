// =====================================================================================
// 
//       Filename:  Trade.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  29/09/09 19:10:09
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "trading/Trade.class.h"

using namespace GLOOPER_TEST;

unsigned long Trade::instance_ctr(0);

Trade::Trade(unsigned long q_,double p_): SimulationObject(++instance_ctr),
   q(q_), p(p_) {}

XmlField Trade::xml_description() const
{
   XmlField tmp("Trade");
   tmp.add_field("quantity",q);
   tmp.add_field("price",p);
   return tmp;
}
