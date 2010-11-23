N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Uniform with 1000 NA(Uniform)(1,0) agents"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/agents/020e_uni_1000na.sim.cfg.agent.lua"