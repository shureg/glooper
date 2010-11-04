execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "public-private information - mixed(uniform) with triangular/triangular"

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0,1,0.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.ppi_mixed_rnd_uni_tri.cfg.lua")
