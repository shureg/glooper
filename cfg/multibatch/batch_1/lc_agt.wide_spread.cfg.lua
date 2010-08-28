glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/lua_complex_agent/?.lua;"..package.path

LuaComplexAgent = require "lua_complex_agent"

LuaComplexAgent.max_spread = 0.1
