N_threads = 4
N_batches = 1
N_runs = 30
N_steps = 500

sim_comment = "public-private information - fixed(1,0) with uniform/unifom (short test)"

info = glooper.SimpleInfoGenerator( rng.UniformGenerator() )

agt_gen_cfg_file = os.path.join(cwd,"agent.ppi_pbl_uni.cfg.lua")
