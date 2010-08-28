execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "bid-ask spread effect - wide spreads"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.wsp.cfg.lua")
