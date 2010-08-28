// =====================================================================================
// 
//       Filename:  AgentPopulation.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:23:24
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  AGENT_POPULATION_CLASS_INC
#define  AGENT_POPULATION_CLASS_INC

#include "boost/shared_ptr.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include <string>
#include <list>
#include "process/Process.class.h"
#include "xml_serialisation/XmlContainerWrap.class.h"

#include "agent/AgentGenerator.class.h"
#include "trading/Market.class.h"
#include "info/InfoGenerator.class.h"
#include <set>

using namespace RNG;

namespace GLOOPER_TEST
{
   class AgentPopulation: public Process
   {
   public:

      AgentPopulation(
	    const boost::shared_ptr< InfoGenerator >&,
	    const boost::shared_ptr< AgentGenerator >&,
	    const boost::shared_ptr< Market >&);

      //! A custom copy constructor
      /*! Must ensure that thread-specific heap-allocated 
       *  data members are cloned, rather than simply copied
       */
      AgentPopulation(const AgentPopulation&);

      void evolve();

      unsigned long get_agent_timer(bool);

      void simulation_config();

      void batch_config();

      void run_config();

      void thread_config();

      const std::string agt_gen_description(const std::string& fmt) const;

      const std::string info_description(const std::string& fmt) const;

      const std::string mkt_description(const std::string& fmt) const;

   protected:

      boost::shared_ptr< AgentGenerator> agt_gen;

      double last_info;

      boost::shared_ptr< InfoGenerator > info_generator;

      boost::shared_ptr< Market > mkt;

      boost::ptr_vector<Agent> population;

      std::vector<unsigned int> population_index;

      boost::ptr_vector<Agent> initial_population;

      unsigned long agent_timer;

      unsigned long turn_timer;

      Process* real_clone() const;

      XML_SERIALISATION::XmlContainerWrap agent_population_description
	 (const std::string&,
	    const boost::ptr_vector<Agent>&) const;

   };
}

#endif   // ----- #ifndef AGENT_POPULATION_CLASS_INC -----
