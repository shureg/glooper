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

      virtual ~Agent(){}

      Agent(double);

      virtual void update_belief(double) = 0;

      virtual void place_order() = 0;

      virtual XmlField xml_description() const;

      virtual void position_update(const Order&) = 0;

      virtual bool can_trade() = 0;

      boost::shared_ptr<Market> get_market() const;

      Agent& set_market(const boost::shared_ptr<Market>&);

      Agent* clone() const;

      Agent& set_timer(const boost::shared_ptr< timer_signal >&);
      
      Agent& set_ro_timer(const boost::shared_ptr< timer_signal >&);

      virtual void init() = 0;

   protected:

      static unsigned long instance_counter;

      double belief;

      boost::shared_ptr<Market> spot_mkt;

      boost::shared_ptr< timer_signal > timer;

      boost::shared_ptr< timer_signal > ro_timer;

      virtual const char* agent_type_str() const = 0;

      virtual Agent* real_clone() const = 0;
   };

   //! Required for Boost Pointer Container library to enable deep copy 
   inline Agent* new_clone( const Agent& a )
   {
      return a.clone();
   }
}

#endif   // ----- #ifndef _GLT_AGENT_CLASS_INC  -----
