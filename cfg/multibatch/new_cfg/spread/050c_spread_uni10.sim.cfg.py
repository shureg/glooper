N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Uniform with 100 NA(Uniform)(1,0) agents; Uniform(0.001, 0.1) "

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/spread/050c_spread_uni10.sim.cfg.agent.lua"