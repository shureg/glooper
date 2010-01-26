import os
import os.path
import sys
import sedna

DATA_PATH = "/simulation_data/data/db"
XQUERY_LIB_PATH = "/simulation_data/glooper/lib/python"

version_string = "6.2.2"

simid = 1017

root_datadir = os.path.join(DATA_PATH,version_string,str(simid))
csv_datadir = os.path.join(DATA_PATH,version_string,str(simid),"csv")

os.system("rm -rfv %s" % root_datadir)
os.system("mkdir -pv %s" % root_datadir)
os.system("mkdir -pv %s" % csv_datadir)

query_labels = ["agt","trd","inf","ord","lob"]

for ql in query_labels:
   os.system("mkdir -pv %s" % os.path.join(DATA_PATH,version_string,str(simid),"csv",ql))

conn = None

batch_run_structure = [100,]

nb = len(batch_run_structure)

sys.path.append(XQUERY_LIB_PATH)

import simdb_xquery

os.system("se_sm SimulationDB")

# Connect to the SimulationDB database which is located on localhost
conn = sedna.SednaConnection('localhost', 'SimulationDB','SYSTEM','MANAGER')

# Execute query ...
for b_id in xrange(nb):
   for r_id in xrange(batch_run_structure[b_id]):
      for ql in query_labels:
	 xq_dict = {"xq_label": ql, "xpath_root": 'doc("SimulationDB")/SimulationData', "sim_id": simid, "bat_id": b_id, "run_id": r_id}
	 conn.beginTransaction()
	 print b_id, r_id, ql
	 conn.execute( simdb_xquery.get_xquery(xq_dict) )
	 outfilename = os.path.join(csv_datadir,ql,"%(xq_label)s.%(sim_id)d.%(bat_id)d.%(run_id)d.csv" % xq_dict)
	 outfile = open(outfilename,'w') 
	 for res in conn.resultSequence():
	    outfile.write(res)
	 conn.endTransaction('commit')

# Check connection status and close it
if conn != None and conn.status() == 'ok':
   conn.close()

os.system("se_smsd SimulationDB")
