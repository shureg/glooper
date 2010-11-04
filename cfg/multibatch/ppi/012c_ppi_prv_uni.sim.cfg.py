execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "public-private information - fixed(0,1) with uniform/uniform"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.ppi_prv_uni.cfg.lua")
