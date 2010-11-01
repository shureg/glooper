// =====================================================================================
// 
//       Filename:  LuaComplexAgent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17/02/10 16:28:08
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/LuaComplexAgent.class.h"
#include <fstream>

using namespace GLOOPER_TEST;

LuaComplexAgent::LuaComplexAgent(double belief,
      double wealth,
      const TRG_d& spread_generator,
      int mean_reversion,
      unsigned long max_memory,
      unsigned long significance_threshold,
      const char* lua_cfg_filename): 
   ComplexAgent(belief,wealth,spread_generator,
	 mean_reversion,max_memory,significance_threshold),
   lua_cfg_filename(lua_cfg_filename), L(0)
{
}

LuaComplexAgent::~LuaComplexAgent()
{
}

void LuaComplexAgent::reconfigure()
{
   //if(L!=0) lua_close(L);

   L = luaL_newstate();
   luaL_openlibs(L);
   luaL_dofile(L,lua_cfg_filename);
   
   lua_getglobal(L,"LuaComplexAgent");

   if( lua_isnil(L,-1) )
      LOG(EXCEPTION,boost::format
	    ("The \"LuaComplexAgent\" table is not defined in Lua file %s\n")
	    % lua_cfg_filename
	    );
   if( !lua_istable(L,-1) )
      LOG(EXCEPTION,boost::format
	    ("The \"LuaComplexAgent\" reference in Lua file %s must be a table\n")
	    % lua_cfg_filename
	    );
   
   lua_pushstring(L,"update_belief");
   lua_rawget(L,-2);

   if( lua_isnil(L,-1) )
      LOG(EXCEPTION, boost::format
	    ("The \"LuaComplexAgent\" table in Lua file %s does not have an "\
	     "update_belief\" field\n") % lua_cfg_filename
	    );

   if( !lua_isfunction(L,-1) )
      LOG(EXCEPTION, boost::format
	    ("The \"update_belief\" field in the \"LuaComplexAgent\" table "\
	     "in Lua file %s is not a function\n") % lua_cfg_filename
	    );

   bel_upd_luaref = luaL_ref(L,LUA_REGISTRYINDEX);

   lua_pushstring(L,"adjust_belief");
   lua_rawget(L,-2);

   if( lua_isnil(L,-1) )
      LOG(EXCEPTION, boost::format
	    ("The \"ComplexAgent\" table in Lua file %s does not have an "\
	     "adjust_belief\" field\n") % lua_cfg_filename
	    );

   if( !lua_isfunction(L,-1) )
      LOG(EXCEPTION, boost::format
	    ("The \"adjust_belief\" field in the \"ComplexAgent\" table "\
	     "in Lua file %s is not a function\n") % lua_cfg_filename
	    );
   
   bel_adj_luaref = luaL_ref(L,LUA_REGISTRYINDEX);
   
   obj_ref = luaL_ref(L,LUA_REGISTRYINDEX);
}

void LuaComplexAgent::update_belief(double xi)
{
   lua_rawgeti(L,LUA_REGISTRYINDEX,bel_upd_luaref);
   lua_pushnumber(L,xi);
   lua_pushnumber(L,belief);
   int res = lua_pcall(L,2,1,0);
   if (res != 0)
      LOG(EXCEPTION,boost::format
	    ("LuaComplexAgent %d: could not call LuaComplexAgent.update_belief: "\
	     "%s\n") % id % (lua_tostring(L,-1))
	    );
   belief = lua_tonumber(L,-1);
   lua_pop(L,1);
}

void LuaComplexAgent::adjust_belief(double p_more_extreme)
{
   lua_rawgeti(L,LUA_REGISTRYINDEX,bel_adj_luaref);
   lua_pushnumber(L,p_more_extreme);
   lua_pushnumber(L,belief);
   lua_pushboolean(L,(bool) mean_reverter);
   lua_pushboolean(L, (distr_bytime.back().ratio > 1.) );
   int res = lua_pcall(L,4,1,0);
   if (res != 0)
      LOG(EXCEPTION,boost::format
	    ("LuaComplexAgent %d: could not call LuaComplexAgent.adjust_belief: "\
	     "%s\n") % id % (lua_tostring(L,-1))
	    );
   belief = lua_tonumber(L,-1);
   lua_pop(L,1);
}

const boost::logic::tribool LuaComplexAgent::next_mode() const
{
   return mean_reverter;
}

Agent* LuaComplexAgent::real_clone() const
{
   LuaComplexAgent* tmp = new LuaComplexAgent(*this);

   return (Agent*) tmp;
}

const char* LuaComplexAgent::agent_type_str() const
{
   return "LuaComplexAgent";
}

XmlField LuaComplexAgent::xml_description() const
{
   XmlField tmp = ComplexAgent::xml_description();

//   tmp.add_field("Lua.Config.File.Contents",
//	 lua_cfg_file_content.begin(),lua_cfg_file_content.end(),"");

   return tmp;
}
