execfile(os.path.join(cwd,"common_1000.cfg.py"))

sim_comment = "agent effects: 2000 agents, triangular information"

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0,1,0.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.2000_NA.cfg.lua")
