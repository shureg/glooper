execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "agent effects: 50 MR agents, triangular information"

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0,1,0.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.50_MR.cfg.lua")
