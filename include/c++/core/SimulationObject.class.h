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
#include "boost/signals2.hpp"
#include <algorithm>
#include "boost/bind.hpp"
#include "boost/thread.hpp"

using XML_SERIALISATION::XmlSerialisableObject;
using XML_SERIALISATION::XmlField;

typedef boost::signals2::signal< void(const XmlSerialisableObject&) >
   db_sig_type;

typedef boost::thread_specific_ptr< db_sig_type > db_sig_ptr_type;

namespace GLOOPER_TEST
{
   class SimulationObject: public XmlSerialisableObject
   {
   public:

      SimulationObject(unsigned long);

      static db_sig_type& db_signal();

      template<class CONT> void db_container_signal(const CONT&);

      unsigned long get_id() const;

      static void init_sig_ptr();

   protected:

      unsigned long id;

      static db_sig_ptr_type sig_ptr;

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
