cwd = "/simulation_data/glooper/cfg/multibatch/test_batch"

sim_comment = "base case"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

N_runs = 10
N_steps = 20

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
