require "rng_lua_swig_wrap"

glooper_path = "/simulation_data/glooper"

package.path = glooper_path.."/lib/lua/?.lua;"..package.path
package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

require "glooper_lua_swig_wrap"
require "agent_generator"

glooper = glooper_lua_swig_wrap
rng = rng_lua_swig_wrap
agt_gen = agent_generator

agt_gen.generator = agt_gen.fixed_number_agent_generator
agt_gen.N_agents = 40
agt_gen.agent_ctor = glooper.StandardComplexAgent
agt_gen.generator_args = {
   belief = rng.UniformGenerator(),
   wealth = rng.ParetoGenerator(10000,1.4)
}
agt_gen.non_generator_args = {
   mean_reversion = 0,
   max_memory = 100,
   significance_threshold = 20,
   spread_generator = rng.UniformGenerator(0.001,0.01),
   private_info_generator = rng.UniformGenerator(0,1),
   public_weight = 1,
   private_weight = 1
}

--agt_gen.mixed_generator.table = {
--   {
--      generator = agt_gen.fixed_number_agent_generator,
--      N_agents = 40,
--      agent_ctor = glooper.StandardComplexAgent,
--      generator_args = {
--	 belief = rng.UniformGenerator(),
--	 wealth = rng.ParetoGenerator(10000,1.4)
--      },
--      non_generator_args = {
--	 mean_reversion = 0,
--	 max_memory = 100,
--	 significance_threshold = 20,
--	 spread_generator = rng.UniformGenerator(0.001,0.01)
--      }
--  }--,
--   {
--      generator = agt_gen.fixed_number_agent_generator,
--      N_agents = 20,
--      agent_ctor = glooper.NoiseTrader,
--      generator_args = {
--	 belief = rng.UniformGenerator(),
--	 wealth = rng.ParetoGenerator(10000,1.4)
--      },
--      non_generator_args = {
--	 belief_generator = rng.UniformGenerator(),
--	 spread_generator = rng.UniformGenerator(0.001,0.01)
--      }
--   }
--}

generator = agt_gen.fixed_number_agent_generator
