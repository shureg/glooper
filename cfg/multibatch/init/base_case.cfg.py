cwd = "/simulation_data/glooper/cfg/multibatch/init"

sim_comment = "base case"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

N_runs = 100
N_steps = 1000

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
