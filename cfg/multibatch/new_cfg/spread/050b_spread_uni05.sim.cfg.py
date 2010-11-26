N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Uniform with 100 NA(Uniform)(1,0) agents; Uniform(0.001, 0.05) spread"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/spread/050b_spread_uni05.sim.cfg.agent.lua"