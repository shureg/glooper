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

boost::signal<void (const XmlSerialisableObject&) >& SimulationObject::db_signal()
{
   static boost::signal< void(const XmlSerialisableObject&) > tmp;
   return tmp;
}

SimulationObject::SimulationObject(unsigned long _id): id(_id)
{}
