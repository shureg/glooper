execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "agent effects: 200 agents, uniform information"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.200_NA.cfg.lua")
