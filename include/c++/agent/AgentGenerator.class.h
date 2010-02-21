// =====================================================================================
// 
//       Filename:  AgentGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/10/09 16:35:33
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  AGENTGENERATOR_CLASS_INC
#define  AGENTGENERATOR_CLASS_INC

#include "core/SimulationObject.class.h"
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "agent/Agent.class.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "rng/generator/RandomGenerator.class.h"
#include "boost/unordered_map.hpp"
#include <utility>
#include <list>

typedef std::pair<std::string, RNG::RandomGenerator* > 
   gen_key_type;

typedef boost::shared_ptr< GLOOPER_TEST::timer_signal > bsp_ts;

namespace GLOOPER_TEST
{
   typedef struct
   {
      Agent* agt;
      std::list<gen_key_type> gen_keys;
      bool last;
   } agent_generation_context;

   class AgentGenerator: public SimulationObject
   {
   public:
      
      virtual ~AgentGenerator();

      AgentGenerator(const char*);

      void generate();

      const boost::ptr_vector< Agent >& get_agt_vector() const;

      XmlField xml_description() const;

   protected:

      const char* cfg_filename;

      virtual void load_cfg_file() = 0;

      void add_Agent(const Agent&);

      void register_generator(
	    const gen_key_type&,
	    const Agent& agent);

      virtual agent_generation_context 
	 get_next_agt() = 0;

      boost::ptr_vector< Agent > agt_vector;

      boost::unordered_map< 
	 gen_key_type,
	 std::list< unsigned long > > 
	    generator_registry;

   };
}

#endif   // ----- #ifndef AGENTGENERATOR_CLASS_INC  -----
