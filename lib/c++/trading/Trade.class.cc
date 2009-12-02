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

Trade::Trade(unsigned long q_,double p_, 
      unsigned long active_id_, unsigned long passive_id_): 
   SimulationObject(++instance_ctr),
   q(q_), p(p_), active_id(active_id_), passive_id(passive_id_) {}

XmlField Trade::xml_description() const
{
   XmlField tmp("Trade");
   tmp("id") = id;
   tmp.add_field("quantity",q);
   tmp.add_field("price",p);
   tmp.add_field("active_id",active_id);
   tmp.add_field("passive_id",passive_id);
   return tmp;
}
