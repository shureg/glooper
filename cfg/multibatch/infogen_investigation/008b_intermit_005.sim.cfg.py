execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "Intermittent triangular info (0.05)"

info = glooper.SimpleIntermittentInfoGenerator( rng.TriangularGenerator(0,1,0.5), 0.05 )

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
