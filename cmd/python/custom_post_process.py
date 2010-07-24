#!/usr/bin/env python

import os
import sedna
import sys

PROJECT_ROOT = "/simulation_data"

LOG_PATH = "/simulation_data/data/log"

XQUERY_LIB_PATH = "/simulation_data/glooper/lib/python"

DATA_PATH = "/simulation_data/data/db"

try:

#configuration file must provide the value for simid, version_string and query_labels variables
   execfile("custom_pp.cfg.py")

   os.system("se_sm SimulationDB")

   sys.path.append( os.path.join(PROJECT_ROOT,"glooper/swig/output") )

   #import callback_log_python_swig_wrap as cbl

   #log_filename = os.path.join(LOG_PATH, "custom_post_process", "pp.log" )
   #os.system("mkdir -pv %s" % os.path.join(LOG_PATH,"custom_post_process"))
   #os.system("ln -sf %s %s" % ( log_filename,os.path.join(LOG_PATH,"current_log") ) )

   #F = cbl.FileCallbackLog(log_filename,"glooper",cbl.TRACE)

   print("Beginning custom post-processing\n")

   root_datadir = os.path.join(DATA_PATH,version_string,str(simid))
   csv_datadir = os.path.join(DATA_PATH,version_string,str(simid),"csv")

   os.system("mkdir -pv %s" % root_datadir)
   os.system("mkdir -pv %s" % csv_datadir)

   for ql in query_labels:
      os.system("mkdir -pv %s" % os.path.join(DATA_PATH,version_string,str(simid),"csv",ql))

   conn = None

   brs_vector_filename = os.path.join(DATA_PATH,version_string,str(simid),"brs.vector")

   brs_vector_file = open(brs_vector_filename,'r')

   batch_run_structure = map(lambda z: int(z), ( ( brs_vector_file.read() ).rstrip('\n') ).split('\n') )

   brs_vector_file.close()

   nb = len(batch_run_structure)

   sys.path.append(XQUERY_LIB_PATH)

   import simdb_xquery

   # Connect to the SimulationDB database which is located on localhost
   conn = sedna.SednaConnection('localhost', 'SimulationDB','SYSTEM','MANAGER')
   print("Connection to SimulationDB has been successfully established\n")

   # Execute query ...
   for b_id in xrange(nb):
      for r_id in xrange(batch_run_structure[b_id]):
	 for ql in query_labels:
	    xq_dict = {"xq_label": ql, "xpath_root": 'doc("SimulationDB")/SimulationData', "sim_id": simid, "bat_id": b_id, "run_id": r_id}
	    print("Starting xquery for label %(xq_label)s for batch %(bat_id)d, run id %(run_id)d\n" % xq_dict)
	    conn.beginTransaction()
	    conn.execute( simdb_xquery.get_xquery(xq_dict) )
	    outfilename = os.path.join(csv_datadir,ql,"%(xq_label)s.%(sim_id)d.%(bat_id)d.%(run_id)d.csv" % xq_dict)
	    print("Writing query result to file %s\n" % outfilename)
	    outfile = open(outfilename,'w') 
	    for res in conn.resultSequence():
	       outfile.write(res)
	    conn.endTransaction('commit')

   # Check connection status and close it
   if conn != None and conn.status() == 'ok':
      conn.close()

   print("Custom post-processing complete\n")

   os.system("se_smsd SimulationDB")

   print("All operations complete\n")

except sedna.SednaException, ex:
   raise ex
except Exception, ex:
   raise ex
