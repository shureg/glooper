execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "varying wealth allocation - pareto(1.0) wealth"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.unequal_wealth.cfg.lua")
