glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/?.lua;"..package.path
package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

require "glooper_lua_swig_wrap"
require "rng_lua_swig_wrap"
require "agent_generator"

glooper = glooper_lua_swig_wrap
rng = rng_lua_swig_wrap
agt_gen = agent_generator

agt_gen.agent_ctor = glooper.StandardComplexAgent

agt_gen.generator_args = {
   belief = rng.UniformGenerator(),
   wealth = rng.ParetoGenerator(10000,1.4)
}

agt_gen.non_generator_args = {
   private_info_generator = rng.UniformGenerator(),
   spread_generator = rng.UniformGenerator(0.001,0.01),
   mean_reversion = 0,
   max_memory = 100,
   significance_threshold = 20,
   public_weight = 1,
   private_weight =1
}

agt_gen.N_agents = 100

generator = agt_gen.fixed_number_agent_generator
