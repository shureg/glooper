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
#include <algorithm>
#include "boost/bind.hpp"

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

      template<class CONT> void db_container_signal(const CONT&);

      unsigned long get_id() const;

   protected:

      unsigned long id;

   };

   template<class CONT> 
      inline void SimulationObject::db_container_signal(const CONT& _cont)
      {
	 using boost::bind;

	 std::for_each(_cont.begin(), _cont.end(), 
	       bind(bind(&SimulationObject::db_signal),_1) );
      }
}

#endif   // ----- #ifndef _GLT_SIMULATIONOBJECT_CLASS_INC  -----
