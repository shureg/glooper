glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/?.lua;"..package.path
package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

require "glooper_lua_swig_wrap"
require "rng_lua_swig_wrap"
require "agent_generator"

glooper = glooper_lua_swig_wrap
rng = rng_lua_swig_wrap
agt_gen = agent_generator

agt_gen.agent_ctor = glooper.HomogeneousAgent

agt_gen.generator_args = {
   belief = rng.UniformGenerator(),
}

agt_gen.non_generator_args = {
   order_size = 100,
   min_spread = 0.001,
   max_spread = 0.05
}

agt_gen.N_agents = 100

generator = agt_gen.fixed_number_agent_generator
