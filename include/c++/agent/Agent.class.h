// =====================================================================================
// 
//       Filename:  Agent.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:30:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  _GLT_AGENT_CLASS_INC
#define  _GLT_AGENT_CLASS_INC

#include "trading/Market.class.h"
#include "boost/shared_ptr.hpp"
#include "core/SimulationObject.class.h"

namespace GLOOPER_TEST
{
   typedef boost::signal< unsigned long () > timer_signal;

   class Agent: public SimulationObject
   {
   public:

      Agent(double,
	    boost::shared_ptr<Market>,
	    boost::shared_ptr<timer_signal>,
	    boost::shared_ptr<timer_signal>);

      virtual void update_belief(double) = 0;

      virtual void place_order() = 0;

      virtual XmlField xml_description() const;

      virtual void position_update(const Order&) = 0;

      virtual void add_order(Order*) = 0;

      virtual bool can_trade() = 0;

      Agent* clone() const;

   protected:

      static unsigned long instance_counter;

      boost::shared_ptr<Market> spot_mkt;

      double belief;

      boost::shared_ptr< timer_signal > timer;

      boost::shared_ptr< timer_signal > ro_timer;

      virtual Agent* real_clone() const = 0;
   };
}

#endif   // ----- #ifndef _GLT_AGENT_CLASS_INC  -----
