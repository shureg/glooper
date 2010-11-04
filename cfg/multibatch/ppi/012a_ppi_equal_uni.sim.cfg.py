execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "public-private information - fixed(0.5,0.5) with uniform/uniform"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.ppi_equal_uni.cfg.lua")
