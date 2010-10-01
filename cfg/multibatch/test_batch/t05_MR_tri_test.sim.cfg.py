N_threads = 4
N_batches = 1
N_runs = 10
N_steps = 30

sim_comment = "testing the mean reversion"

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0,1,0.5) )

agt_gen_cfg_file = os.path.join(cwd,"agent.test.MR.cfg.lua")
