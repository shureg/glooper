execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "information generator effect - triangular generator"

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0,1,0.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.base.cfg.lua")
