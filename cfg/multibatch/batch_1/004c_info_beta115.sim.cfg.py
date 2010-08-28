execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "information generator effect - beta(11.5)"

info = glooper.SimpleInfoGenerator( rng.BetaGenerator(11.5,11.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.base.cfg.lua")
