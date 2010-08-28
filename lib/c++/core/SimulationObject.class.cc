// =====================================================================================
// 
//       Filename:  SimulationObject.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  15/06/09 17:12:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "core/SimulationObject.class.h"

using namespace GLOOPER_TEST;

db_sig_ptr_type SimulationObject::sig_ptr;

db_sig_type& SimulationObject::db_signal()
{
   return *(SimulationObject::sig_ptr.get());
}

SimulationObject::SimulationObject(unsigned long _id): id(_id)
{
   init_sig_ptr();
}

void SimulationObject::init_sig_ptr()
{
   if(sig_ptr.get() == 0)
   {
      LOG(TRACE, boost::format(
	       "Assigning thread-specific signal ptr for thread %s\n")
	    % (boost::this_thread::get_id())
	    );
      sig_ptr.reset(new db_sig_type);
   }
}

unsigned long SimulationObject::get_id() const
{
   return id;
}
