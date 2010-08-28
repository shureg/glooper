execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "base case - uniform information generation"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.base.cfg.lua")
