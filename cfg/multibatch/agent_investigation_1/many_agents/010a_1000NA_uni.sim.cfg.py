execfile(os.path.join(cwd,"common_1000.cfg.py"))

sim_comment = "agent effects: 1000 agents, uniform information"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.1000_NA.cfg.lua")
