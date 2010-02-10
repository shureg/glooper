// =====================================================================================
// 
//       Filename:  LuaAgentGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/02/10 14:31:23
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  LUAAGENTGENERATOR_CLASS_INC
#define  LUAAGENTGENERATOR_CLASS_INC

#include "AgentGenerator.class.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace GLOOPER_TEST
{
   class LuaAgentGenerator: public AgentGenerator
   {
   public:

      LuaAgentGenerator(const char* _cfg_filename);

      ~LuaAgentGenerator();

   protected:

      lua_State* L;

      void load_cfg_file();

      agent_generation_context get_next_agt();

   };
}

#endif   // ----- #ifndef LUAAGENTGENERATOR_CLASS_INC  -----
