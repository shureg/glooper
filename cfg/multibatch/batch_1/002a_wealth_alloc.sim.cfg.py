execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "varying wealth allocation - uniform wealth"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.uniwealth.cfg.lua")
