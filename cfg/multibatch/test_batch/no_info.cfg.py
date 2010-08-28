cwd = "/simulation_data/glooper/cfg/multibatch/test_batch"

sim_comment = "no info"

info = glooper.SimpleInfoGenerator( rng.ConstGenerator_double(-1) )

N_runs = 10
N_steps = 20

agt_gen_cfg_file = os.path.join(cwd,"agent.cfg.lua")
