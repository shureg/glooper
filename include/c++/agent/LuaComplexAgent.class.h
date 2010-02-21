// =====================================================================================
// 
//       Filename:  LuaComplexAgent.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17/02/10 16:26:53
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  LUACOMPLEXAGENT_CLASS_INC
#define  LUACOMPLEXAGENT_CLASS_INC

#include "agent/ComplexAgent.class.h"
#include <string>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace GLOOPER_TEST
{
   class LuaComplexAgent: public ComplexAgent
   {
   public:

      LuaComplexAgent(const Market& spot_mkt,
	    double belief,
	    double wealth,
	    unsigned long max_memory,
	    unsigned long significance_threshold,
	    const char* lua_cfg_filename);

      ~LuaComplexAgent();

      void update_belief(double xi);

      XmlField xml_description() const;

   protected:

      lua_State* L;

      int obj_ref;

      int bel_adj_luaref, bel_upd_luaref;

      void adjust_belief(double p_more_extreme);

      const boost::logic::tribool next_mode() const;

      std::string lua_cfg_file_content;

      const char* agent_type_str() const;

      Agent* real_clone() const;
   };
}

#endif   // ----- #ifndef LUACOMPLEXAGENT_CLASS_INC  -----
