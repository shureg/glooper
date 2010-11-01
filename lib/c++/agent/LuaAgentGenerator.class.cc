// =====================================================================================
// 
//       Filename:  LuaAgentGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/02/10 14:34:49
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/LuaAgentGenerator.class.h"
#include <iostream>

using GLOOPER_TEST::LuaAgentGenerator;
using GLOOPER_TEST::agent_generation_context;
using std::string;

LuaAgentGenerator::LuaAgentGenerator(const char* _cfg_filename):
   AgentGenerator(_cfg_filename)
{
}

LuaAgentGenerator::~LuaAgentGenerator()
{
}

void LuaAgentGenerator::load_cfg_file()
{
   L = luaL_newstate();
   luaL_openlibs(L);

   if( luaL_dofile(L,cfg_filename) != 0 )
      LOG(EXCEPTION, boost::format(
	       "LuaAgentGenerator: could not load the Lua config file %s: %s\n")
	    % cfg_filename % (lua_tostring(L,-1))
	    );

   lua_getglobal(L,"generator");

   if(lua_isnil(L,-1))
      LOG(EXCEPTION, boost::format(
	       "LuaAgentGenerator: there is no global with name "\
	       "\"generator\"\n")
	    );

   if(!lua_isfunction(L,-1))
      LOG(EXCEPTION, boost::format(
	       "LuaAgentGenerator: the global \"generator\" is of "\
	       "the wrong type: expected function, got %s\n")
	    % (lua_typename(L,lua_type(L,-1)))
	    );

   gen_ref = luaL_ref(L,LUA_REGISTRYINDEX);
}

agent_generation_context LuaAgentGenerator::get_next_agt()
{
   agent_generation_context tmp;

   lua_rawgeti(L,LUA_REGISTRYINDEX,gen_ref);

   int res = lua_pcall(L,0,4,0);
   
   if(res != 0)
      LOG(EXCEPTION, boost::format(
	       "LuaAgentGenerator: unable to call the agent "\
	       "generating function: %s\n")
	    % (lua_tostring(L,-1))
	    );

   LOG(TRACE, boost::format(
	    "Completed agent generating call\n")
	 );

   tmp.last = lua_toboolean(L,-1);
   int agt_ctr = lua_tointeger(L,-2);

   LOG(TRACE, boost::format(
	    "At agent counter %d, the stopping condition is %d\n")
	 % agt_ctr % (int) tmp.last
	 );

   lua_pop(L,2);

   if( lua_istable(L,-1) == 0 )
      LOG(EXCEPTION, boost::format(
	       "LuaAgentGenerator: (after %d generation invocations) "\
	       "the second value returned by the call to generate() "\
	       "does not appear to be a table\n") % agt_ctr
	    );

   int N = lua_objlen(L,-1);

   LOG(TRACE, boost::format(
	    "LuaAgentGenerator: after %d generator invocations "\
	    "the generator table length is %d\n") % agt_ctr % N
	 );

   for(int i = 0; i<N; ++i)
   {
      lua_pushinteger(L,i+1);

      lua_rawget(L,-2);

      if(!lua_istable(L,-1))
	 LOG(EXCEPTION,boost::format(
		  "LuaAgentGenerator: (after %d generation invocations) "\
		  "the value at index %d in the generator table "\
		  "is not a table\n") % agt_ctr % (i+1)
	       );

      lua_pushstring(L,"lbl");
      lua_rawget(L,-2);

      if( lua_isstring(L,-1) == 0 )
	 LOG(EXCEPTION,boost::format
	       ("LuaAgentGenerator: (after %d generation invocations) "\
		"The value at index [\"lbl\"] "\
		  "in generator list element %d does not appear to be "\
		  "convertible to string\n") % agt_ctr % (i+1)
	       );
      string lbl = lua_tostring(L,-1);

      lua_pop(L,1);

      lua_pushstring(L,"gen");
      lua_rawget(L,-2);

      lua_getfield(L,-1,".this");

      if( lua_islightuserdata(L,-1) == 0 )
	 LOG(EXCEPTION,boost::format
	       ("LuaAgentGenerator: (after %d generation invocations) "\
		"The value at index [\"gen\"][\".this\"] "\
		  "in generator list element %d does not appear to be "\
		  "a lightuserdata\n") % agt_ctr % (i+1)
	       );
      RNG::RandomGenerator* gen = 
	 (RNG::RandomGenerator*) lua_touserdata(L,-1);

      tmp.gen_keys.push_back(gen_key_type(lbl,gen));

      LOG(TRACE,boost::format(
	       "LuaAgentGenerator: (after %d generation invocations) "\
	       "check-inserting generator with label %s at %#x to the generator "\
	       "registry\n") % agt_ctr % lbl % gen
	    );

      lua_pop(L,3);
   }

   lua_pop(L,1);

   lua_getfield(L,-1,".this");

   if( lua_islightuserdata(L,-1) == 0 )
      LOG(EXCEPTION,boost::format
	    ("LuaAgentGenerator: (after %d generation invocations) "\
	     "The value at index [\".this\"] "\
	       "of the first result of generate() call does not appear to be "\
	       "a lightuserdata\n") % agt_ctr
	    );
   
   tmp.agt = (Agent*) lua_touserdata(L,-1);
   lua_pop(L,1);

   return tmp;
}
