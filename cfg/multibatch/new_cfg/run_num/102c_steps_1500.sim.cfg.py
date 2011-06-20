N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1500

sim_comment = "Uniform with 100 NA(Uniform)(1,0) agents - 1500 steps"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/run_num/102c_steps_1500.sim.cfg.agent.lua"