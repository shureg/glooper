execfile(os.path.join(cwd,"common.cfg.py"))

sim_comment = "(Intermittent) triangular Markovian info (0.01)"

info = glooper.TriangularMarkovianInfoGenerator( 0.01 )

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
