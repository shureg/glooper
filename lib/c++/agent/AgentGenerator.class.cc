// =====================================================================================
// 
//       Filename:  AgentGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/10/09 16:35:35
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/AgentGenerator.class.h"
#include <algorithm>

using namespace GLOOPER_TEST;
using namespace RNG;
using std::list;

AgentGenerator::AgentGenerator(const char* _cfg_filename):
   SimulationObject(0), cfg_filename(_cfg_filename)
{}

void AgentGenerator::generate()
{
   load_cfg_file();

   bool stop = false;

   while(!stop)
   {
      agent_generation_context agc = get_next_agt();
      add_Agent(*(agc.agt));
      for(list<gen_key_type>::const_iterator
	    i = agc.gen_keys.begin();
	    i != agc.gen_keys.end();
	    ++i)
	 register_generator(*i, *(agc.agt));
      stop = agc.last;
   }
}

void AgentGenerator::add_Agent(const Agent& _agt)
{
   agt_vector.push_back(_agt.clone());
}

const boost::ptr_vector< Agent >& AgentGenerator::get_agt_vector() const
{
   return agt_vector;
}

void AgentGenerator::register_generator(
      const gen_key_type& gen_key,
      const Agent& agt)
{
   generator_registry[ gen_key ].push_back(agt.get_id());
}

XmlField AgentGenerator::xml_description() const
{
   XmlField tmp("Agent.Generator");

   for (boost::unordered_map<gen_key_type, std::list<unsigned long> >::
	 const_iterator 
	 i = generator_registry.begin(); 
	 i != generator_registry.end();
	 ++i)
   {
      XmlField gen_tmp(std::string(i->first.first)+std::string(".Generator"));
      gen_tmp.add_field(i->first.second->xml_description());
      gen_tmp.add_field("generated_agents",
	    i->second.begin(),i->second.end()," ");
      tmp.add_field(gen_tmp);
   }

   return tmp;
}
