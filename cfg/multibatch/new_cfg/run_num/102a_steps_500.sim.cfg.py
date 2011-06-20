N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 500

sim_comment = "Uniform with 100 NA(Uniform)(1,0) agents - 500 steps"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/run_num/102a_steps_500.sim.cfg.agent.lua"