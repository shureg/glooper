execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "degenerate case - no information generation"

info = glooper.SimpleInfoGenerator( rng.ConstGenerator_double(-1) )

agt_gen_cfg_file = os.path.join(cwd,"agent.base.cfg.lua")
