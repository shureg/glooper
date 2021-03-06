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

thread_wrap<unsigned long> Trade::instance_ctr(0ul);

void Trade::reset_instance_ctr()
{
   Trade::instance_ctr.reset(0ul);
}

Trade::Trade(unsigned long q_,double p_, 
      unsigned long active_id_, unsigned long passive_id_): 
   SimulationObject(++instance_ctr),
   q(q_), p(p_), active_id(active_id_), passive_id(passive_id_) {}

XmlField Trade::xml_description() const
{
   XmlField tmp("Trade");
   tmp("id") = id;
   tmp("quantity") = q;
   tmp("price") = p;
   tmp("active_id") = active_id;
   tmp("passive_id") = passive_id;
   return tmp;
}
