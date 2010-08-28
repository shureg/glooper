execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "information generator effect - beta(2.5)"

info = glooper.SimpleInfoGenerator( rng.BetaGenerator(2.5,2.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.base.cfg.lua")
