execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "(Intermittent) triangular Markovian info (1.0)"

info = glooper.TriangularMarkovianInfoGenerator( 1.0 )

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
