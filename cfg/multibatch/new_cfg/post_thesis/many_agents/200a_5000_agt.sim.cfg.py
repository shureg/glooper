N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Uniform with 5000 NA(Uniform)(1,0) agents "

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/post_thesis/many_agents/200a_5000_agt.sim.cfg.agent.lua"