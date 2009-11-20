// =====================================================================================
// 
//       Filename:  SimulationObject.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  15/06/09 17:07:35
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  _GLT_SIMULATIONOBJECT_CLASS_INC
#define  _GLT_SIMULATIONOBJECT_CLASS_INC

#include "xml_serialisation/XmlSerialisableObject.class.h"
#include "boost/signals.hpp"

using XML_SERIALISATION::XmlSerialisableObject;
using XML_SERIALISATION::XmlField;

namespace GLOOPER_TEST
{
   class SimulationObject: public XmlSerialisableObject
   {
   public:

      SimulationObject(unsigned long);

      static boost::signal <void (const XmlSerialisableObject&) >& 
	 db_signal();

      unsigned long get_id() const;

   protected:

      unsigned long id;

   };
}

#endif   // ----- #ifndef _GLT_SIMULATIONOBJECT_CLASS_INC  -----
