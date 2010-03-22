#!/usr/bin/env python

import os
import sedna
import sys

try:

   os.system("se_sm SimulationDB")

   execfile("glooper.cfg.py")

#Run the simulation

   cbl.log(cbl.INFO,"Beginning simulation %d\n" % simid)

   sim.simulate()

#Simulation data post-processing

   if post_process:

      cbl.log(cbl.INFO,"Simulation complete, beginning post-processing\n")

      root_datadir = os.path.join(DATA_PATH,version_string,str(simid))
      csv_datadir = os.path.join(DATA_PATH,version_string,str(simid),"csv")

      os.system("mkdir -pv %s" % root_datadir)
      os.system("mkdir -pv %s" % csv_datadir)

      query_labels = post_process

      for ql in query_labels:
	 os.system("mkdir -pv %s" % os.path.join(DATA_PATH,version_string,str(simid),"csv",ql))

      conn = None

      batch_run_structure = sim.get_batch_run_structure()

      nb = len(batch_run_structure)

      brs_vector_filename = os.path.join(DATA_PATH,version_string,str(simid),"brs.vector")

      brs_vector_file = open(brs_vector_filename,'w')

      brs_vector_file.write( '\n'.join( map(lambda z: str(z), batch_run_structure)) )

      brs_vector_file.close()

      sys.path.append(XQUERY_LIB_PATH)

      import simdb_xquery

      # Connect to the SimulationDB database which is located on localhost
      conn = sedna.SednaConnection('localhost', 'SimulationDB','SYSTEM','MANAGER')
      cbl.log(cbl.INFO,"Connection to SimulationDB has been successfully established\n")

      # Execute query ...
      for b_id in xrange(nb):
	 for r_id in xrange(batch_run_structure[b_id]):
	    for ql in query_labels:
	       xq_dict = {"xq_label": ql, "xpath_root": 'doc("SimulationDB")/SimulationData', "sim_id": simid, "bat_id": b_id, "run_id": r_id}
	       cbl.log(cbl.INFO,"Starting xquery for label %(xq_label)s for batch %(bat_id)d, run id %(run_id)d\n" % xq_dict)
	       conn.beginTransaction()
	       conn.execute( simdb_xquery.get_xquery(xq_dict) )
	       outfilename = os.path.join(csv_datadir,ql,"%(xq_label)s.%(sim_id)d.%(bat_id)d.%(run_id)d.csv" % xq_dict)
	       cbl.log(cbl.INFO,"Writing query result to file %s\n" % outfilename)
	       outfile = open(outfilename,'w') 
	       for res in conn.resultSequence():
		  outfile.write(res)
	       conn.endTransaction('commit')

      # Check connection status and close it
      if conn != None and conn.status() == 'ok':
	 conn.close()

      cbl.log(cbl.INFO,"Simulation %d post-processing complete\n" % simid)

   os.system("se_smsd SimulationDB")

   cbl.log(cbl.INFO,"All operations complete\n")

except sedna.SednaException, ex:
   raise ex
except Exception, ex:
   raise ex
