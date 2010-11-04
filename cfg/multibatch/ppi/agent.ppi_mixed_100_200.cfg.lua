require "rng_lua_swig_wrap"

glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/?.lua;"..package.path
package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

require "glooper_lua_swig_wrap"
require "agent_generator"

glooper = glooper_lua_swig_wrap
rng = rng_lua_swig_wrap
agt_gen = agent_generator

agt_gen.mixed_generator.table = {
   {
      generator = agt_gen.fixed_number_agent_generator,
      N_agents = 100,
      agent_ctor = glooper.StandardComplexAgent,
      generator_args = {
	 belief = rng.UniformGenerator(),
	 wealth = rng.ParetoGenerator(10000,1.4)
      },
      non_generator_args = {
	 mean_reversion = 0,
	 max_memory = 100,
	 significance_threshold = 20,
	 spread_generator = rng.UniformGenerator(0.001,0.01),
	 private_info_generator = rng.TriangularGenerator(0,1,0.5),
	 public_weight = 1,
	 private_weight = 0
      }
   },
   {
      generator = agt_gen.fixed_number_agent_generator,
      N_agents = 200,
      agent_ctor = glooper.StandardComplexAgent,
      generator_args = {
	 belief = rng.UniformGenerator(),
	 wealth = rng.ParetoGenerator(10000,1.4)
      },
      non_generator_args = {
	 mean_reversion = 0,
	 max_memory = 100,
	 significance_threshold = 20,
	 spread_generator = rng.UniformGenerator(0.001,0.01),
	 private_info_generator = rng.TriangularGenerator(0,1,0.5),
	 public_weight = 0,
	 private_weight = 1
      }
   },
}

generator = agt_gen.mixed_agent_generator
