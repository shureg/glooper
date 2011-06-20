N_threads = 4
N_batches = 1
N_runs = 50
N_steps = 1000

sim_comment = "Uniform with 100 NA(Uniform)(1,0) agents - 50 runs"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/run_num/101a_runs_50.sim.cfg.agent.lua"