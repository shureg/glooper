execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "information generator effect - beta(47.5)"

info = glooper.SimpleInfoGenerator( rng.BetaGenerator(47.5,47.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.base.cfg.lua")
