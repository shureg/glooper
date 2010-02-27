glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/?.lua;"..package.path
package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

local glooper = require "glooper_lua_swig_wrap"
local rng = require "rng_lua_swig_wrap"
local agt_gen = require "agent_generator" 

agt_gen.agent_ctor = glooper.ClassicAgent

agt_gen.generator_args = {
   belief = rng.UniformGenerator(),
   wealth = rng.ParetoGenerator(10000,1.4),
   f_min = rng.UniformGenerator(0.05,0.1),
   p_min = rng.UniformGenerator(0.05,0.1),
   bas_max = rng.UniformGenerator(0.001,0.01)
}

agt.gen.non_generator_args = {}

agt_gen.N_agents = 2

generator = agt_gen.fixed_number_agent_generator
