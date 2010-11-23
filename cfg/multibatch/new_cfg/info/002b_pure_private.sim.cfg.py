N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Uniform with 100 NA(Uniform)(0,1) agents "

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/info/002b_pure_private.sim.cfg.agent.lua"