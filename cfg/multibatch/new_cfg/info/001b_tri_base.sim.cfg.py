N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Triangular with 100 NA(Uniform)(1,0) agents "

info = glooper.SimpleInfoGenerator( rng.TriangularGenerator(0, 1, 0.5) )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/info/001b_tri_base.sim.cfg.agent.lua"