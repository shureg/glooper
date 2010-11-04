execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "public-private information - mixed(100,100) with triangular/triangular"

info = glooper.SimpleIntermittentInfoGenerator( rng.TriangularGenerator(0,1,0.5), 0.05 )

agt_gen_cfg_file = os.path.join(cwd,"agent.ppi_mixed_100_100.cfg.lua")
