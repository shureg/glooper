glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/?.lua;"..package.path
package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

require "glooper_lua_swig_wrap"
require "rng_lua_swig_wrap"
require "agent_generator"

glooper = glooper_lua_swig_wrap
rng = rng_lua_swig_wrap
agt_gen = agent_generator

agt_gen.agent_ctor = glooper.LuaComplexAgent

agt_gen.generator_args = {
   belief = rng.UniformGenerator(),
   wealth = rng.ParetoGenerator(10000,1.4)
}

agt_gen.non_generator_args = {
   mean_reversion = -1,
   max_memory = 100,
   significance_threshold = 20,
   lua_cfg_filename = "lc_agt.cfg.lua"
}

agt_gen.N_agents = 500

generator = agt_gen.fixed_number_agent_generator
