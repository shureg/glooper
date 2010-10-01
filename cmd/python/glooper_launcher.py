#!/usr/bin/env python

import os
import sedna
import sys
import datetime

try:

   #os.system("se_sm SimulationDB")

   custom_settings_file = None

   if len(sys.argv) > 1:
      custom_settings_file = str( sys.argv[1] )

   execfile("glooper.cfg.py")

#Run the simulation

   cbl.log(cbl.INFO,"Beginning simulation %d\n" % simid)

   t0 = datetime.datetime.now()

   sim.simulate()

   t1 = datetime.datetime.now()

   cbl.log(cbl.INFO,"Ending simulation %d\n" % simid)

#Statistical post-processing
   
   batch_run_structure = sim.get_batch_run_structure()

   brs_vector_filename = os.path.join(DATA_PATH,version_string,str(simid),"brs.vector")

   brs_vector_file = open(brs_vector_filename,'w')

   brs_vector_file.write( '\n'.join( map(lambda z: str(z), batch_run_structure)) )

   brs_vector_file.close()

   cbl.log(cbl.INFO,"Beginning statistical post-processing for simulation %d" % simid)

   os.chdir(STATS_PATH)

   os.system("R --slave --no-save --no-restore --args %s %d < process.R" % (os.path.join(DATA_PATH,version_string), simid) )

   cbl.log(cbl.INFO,"Statistical post-processing for simulation %d complete" % simid)

   t2 = datetime.datetime.now()

#Register simulation-specific information

   cbl.log(cbl.INFO,"Beginning registration for simulation %d\n" % simid)
   
   sim_reg_file = os.path.join(DATA_PATH,sim_reg_filename)
   if not os.path.exists(sim_reg_file):
      srf = open(sim_reg_file,'w')
      srf.write( ','.join( 
	 ['library_version','simulation_id','threads','total_batches','total_runs','total_steps','runs_per_batch','steps_per_run','agents',
	    'comment','case_code','multibatch_cfg_file',
	    'info_generator','agent_generators','market',
	    'data_path','log_path',
	    'runtime_seconds','runtime_str','process_time_seconds','process_time_str','total_time_seconds','total_time_str',
	    'timestamp_begin', 'timestamp_end']) + '\n' )
      srf.close()

   tsc = sim.get_total_step_count();

   agt_count = agt_gen.population_size();

   nb = len(batch_run_structure)

   nr = sum(batch_run_structure)

   rt = t1 - t0;

   pt = t2 - t1;

   tt = t2 - t0;

   get_seconds = lambda x: round(x.days*24*60*60 + x.seconds + x.microseconds*1e-6)

   quote_str = lambda x: ('"%s"' % str(x))

   case_code = os.path.basename(custom_settings_file).split("_",2)[0]

   srf = open(sim_reg_file,'a')

   srf.write( ','.join(
      [ version_string, str(simid), str(N_threads), str(nb), str(nr), str(tsc), ("%d" % (1.*nr/nb)), ( "%d" % (1.*tsc/nr) ), str(agt_count) ,
	 quote_str(sim_comment), quote_str(case_code), str(custom_settings_file),
	 ('"%s"' % agt_pop.info_description("Python")), ('"%s"' % agt_pop.agt_gen_description("Python")), ('"%s"' % agt_pop.mkt_description("Python")),  
	 root_datadir, root_logdir, 
	 str( get_seconds(rt) ), quote_str(rt), str( get_seconds(pt) ), quote_str(pt), str( get_seconds(tt) ), quote_str(tt), 
	 str(t0), str(t2)
	 ]) + '\n'
   )

   srf.close()

   cbl.log(cbl.INFO,"Registration for simulation %d complete \n" % simid)


   cbl.log(cbl.INFO,"All operations complete\n")

except Exception, ex:
   raise ex
