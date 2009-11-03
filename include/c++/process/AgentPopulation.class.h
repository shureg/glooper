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

#include "rng/generator/TypedRandomGenerator.class.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include <string>
#include "process/Process.class.h"
#include "xml_serialisation/XmlContainerWrap.class.h"

#include "agent/AgentGenerator.class.h"

using namespace RNG;

namespace GLOOPER_TEST
{
   class Agent;

   class AgentPopulation: public Process
   {
   public:

      AgentPopulation(
	    const boost::shared_ptr< TypedRandomGenerator<double> >&,
	    const boost::shared_ptr< AgentGenerator >&);

      void evolve();

      unsigned long get_agent_timer(bool);

      void simulation_config();

      void batch_config();

      void run_config();

   protected:

      boost::shared_ptr< AgentGenerator> agt_gen;

      double last_info;

      boost::shared_ptr< TypedRandomGenerator<double> > info_generator;

      boost::ptr_vector<Agent> population;

      boost::ptr_vector<Agent> initial_population;

      unsigned long agent_timer;

      unsigned long turn_timer;

      XML_SERIALISATION::XmlContainerWrap agent_population_description
	 (const std::string&,
	    const boost::ptr_vector<Agent>&) const;

   };
}

#endif   // ----- #ifndef AGENT_POPULATION_CLASS_INC -----
