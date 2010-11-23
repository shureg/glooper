N_threads = 4
N_batches = 1
N_runs = 100
N_steps = 1000

sim_comment = "Intermittent(0.1) Uniform with 100 NA(Uniform)(1,0) agents "

info = glooper.SimpleIntermittentInfoGenerator( rng.UniformGenerator(), 0.1 )

agt_gen_cfg_file = "/simulation_data/glooper/cfg/multibatch/new_cfg/info/002e_pub_intermit_010.sim.cfg.agent.lua"