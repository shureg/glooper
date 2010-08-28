cwd = "/simulation_data/glooper/cfg/multibatch/init"

sim_comment = "no info"

info = glooper.SimpleInfoGenerator( rng.ConstGenerator_double(-1) )

N_runs = 100
N_steps = 1000

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
