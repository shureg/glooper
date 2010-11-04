sim_comment = "public-private information test"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

N_runs = 10
N_steps = 20

agt_gen_cfg_file = os.path.join(cwd,"agent.test_test.cfg.lua")

sim_reg_filename = "test_registry.csv"
