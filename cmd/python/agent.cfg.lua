glooper_path = "/home/shureg/Projects/glooper"

dofile(glooper_path.."/lib/lua/agent_generator.lua")

agent_ctor = glooper.ClassicAgent

generator_args = {
   belief = rng.UniformGenerator(),
   wealth = rng.ParetoGenerator(10000,1.4),
   f_min = rng.UniformGenerator(0.05,0.1),
   p_min = rng.UniformGenerator(0.05,0.1),
   bas_max = rng.UniformGenerator(0.001,0.01)
}

non_generator_args = {}

N_agents = 2

generator = fixed_number_agent_generator
