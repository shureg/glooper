N_threads = 4
N_batches = 1
N_runs = 50
N_steps = 1000

sim_comment = "public-private information - fixed(1,0) with alternating/uniform (long test)"

info = glooper.AlternatingInfoGenerator(0.8,0.2)

agt_gen_cfg_file = os.path.join(cwd,"agent.ppi_pbl_uni.cfg.lua")
