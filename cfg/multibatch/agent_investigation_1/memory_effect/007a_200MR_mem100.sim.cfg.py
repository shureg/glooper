execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "memory effects: 200 MR agents, triangular information, memory 100"

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0,1,0.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.200_MR_mem100.cfg.lua")
