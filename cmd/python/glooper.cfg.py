#!/usr/bin/env python

#Filename and pathname related settings

PROJECT_ROOT = "/simulation_data"

LOG_PATH = "/simulation_data/data/log"

VERSION_FILENAME = "glooper.version"

XQUERY_LIB_PATH = "/simulation_data/glooper/lib/python"

DATA_PATH = "/simulation_data/data/db"

STATS_PATH = "/simulation_data/statistics/R"

#Base simid (if no simulation id is found in the database, numbering stats with this value)

BASE_SIMID = 1000

#Module imports

import sys
import os.path
import subprocess

sys.path.append( os.path.join(PROJECT_ROOT,"glooper/swig/output") )

import glooper_python_swig_wrap as glooper
import rng_python_swig_wrap as rng
import callback_log_python_swig_wrap as cbl

#Existence checks

assert os.path.exists(LOG_PATH), "Log path %s does not exist" % LOG_PATH
assert os.path.exists(VERSION_FILENAME), "Version file %s does not exist" % VERSION_FILENAME
assert os.path.exists(XQUERY_LIB_PATH), "Python xquery module path %s does not exist" % XQUERY_LIB_PATH
assert os.path.exists(DATA_PATH), "Data path %s does not exist" % DATA_PATH
assert os.path.exists(STATS_PATH), "Statistical post-processing path %s does not exist" % STATS_PATH

#Simid retrieval
#q_res = subprocess.Popen(['se_term','-query','max(doc("SimulationDB")/SimulationData/Simulation/@id)','SimulationDB'],stdout = subprocess.PIPE)
#simid_str = q_res.communicate()[0].rstrip('\n')
#if simid_str.isdigit():
#   simid = int(simid_str) + 1
#else:
#   simid = BASE_SIMID

simid_file = open("simid",'r')

simid = int( (simid_file.read()).rstrip('\n') ) + 1

simid_file.close()

simid_file = open("simid",'w')

simid_file.write(str(simid))

simid_file.close()

#Version string

version_file = open(VERSION_FILENAME, 'r')
version_string = str(version_file.readline())
version_file.close()

#Log initialisation

root_logdir = os.path.join(LOG_PATH,version_string,str(simid))
log_filename = os.path.join(root_logdir,"sim.%d.log" % simid )
os.system("mkdir -pv %s" % os.path.join(LOG_PATH,version_string,str(simid)))
os.system("ln -sf %s %s" % ( log_filename,os.path.join(LOG_PATH,"current_log") ) )

F = cbl.FileCallbackLog(log_filename,"glooper",cbl.INFO)

#Flat XML file path creation
xml_path = os.path.join(DATA_PATH,version_string,str(simid),"xml")
os.system("mkdir -pv %s" % xml_path)

#Flat CSV file path structure creation 
root_datadir = os.path.join(DATA_PATH,version_string,str(simid))
csv_datadir = os.path.join(DATA_PATH,version_string,str(simid),"csv")

os.system("mkdir -pv %s" % root_datadir)
os.system("mkdir -pv %s" % csv_datadir)

query_labels = ['agt','trd','inf','ord','lob']

for ql in query_labels:
   os.system("mkdir -pv %s" % os.path.join(DATA_PATH,version_string,str(simid),"csv",ql))

#Simulation-related objects creation

#default settings

sim_comment = "final test" #raw_input("Provide a short description for simulation %d: " % simid)

N_batches = 1

N_runs = 100

N_steps = 100

N_threads = 4

post_process = None #["trd","ord","inf","agt"]

mkt = glooper.Market()

igen = rng.UniformGenerator()

info = glooper.SimpleInfoGenerator( igen )

tdb_cfg_file = "table_db.cfg.lua"

agt_gen_cfg_file = "agent.cfg.lua"

# If a custom settings file has been provided, execute it to override default settings
if( custom_settings_file != None):
   cwd = os.path.split(custom_settings_file)[0]
   execfile(custom_settings_file)

dbi = glooper.TableDBInterface(tdb_cfg_file,csv_datadir)

agt_gen = glooper.LuaAgentGenerator(agt_gen_cfg_file)

agt_pop = glooper.AgentPopulation(info,agt_gen,mkt)

sim = glooper.StandardFSSimulation(agt_pop,sim_comment,xml_path,simid,dbi,N_batches,N_runs,N_steps)

sim.set_log_ptr(F)

sim.set_N_threads(N_threads)
