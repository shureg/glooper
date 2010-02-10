#!/usr/bin/env python

#Filename and pathname related settings

PROJECT_ROOT = "/simulation_data"

LOG_PATH = "/simulation_data/data/log"

VERSION_FILENAME = "glooper.version"

XQUERY_LIB_PATH = "/simulation_data/glooper/lib/python"

DATA_PATH = "/simulation_data/data/db"

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
assert os.path.exists(DATA_PATH), "Post-processing data path %s does not exist" % DATA_PATH

#Simid retrieval
q_res = subprocess.Popen(['se_term','-query','max(doc("SimulationDB")/SimulationData/Simulation/@id)','SimulationDB'],stdout = subprocess.PIPE)
simid_str = q_res.communicate()[0].rstrip('\n')
if simid_str.isdigit():
   simid = int(simid_str) + 1
else:
   simid = BASE_SIMID

#Version string

version_file = open(VERSION_FILENAME, 'r')
version_string = str(version_file.readline())
version_file.close()

#Log initialisation

log_filename = os.path.join(LOG_PATH,version_string,str(simid),"sim.%d.log" % simid )
os.system("mkdir -pv %s" % os.path.join(LOG_PATH,version_string,str(simid)))
os.system("ln -sf %s %s" % ( log_filename,os.path.join(LOG_PATH,"current_log") ) )

F = cbl.FileCallbackLog(log_filename,"glooper",cbl.TRACE)

#Simulation-related objects creation

N_agents = 100

N_batches = 1

N_runs = 100

N_steps = 100

sim_comment = "spread_test"

post_process = True

dbi = glooper.SednaDBInterface("SimulationDB","Bender Rodriguez","bmsma_DTAH1","SimulationData")

belief = rng.UniformGenerator()

wealth = rng.UniformGenerator(10000,60000)

f_min = rng.UniformGenerator(0.05,0.1)

p_min = rng.UniformGenerator(0.05,0.1)

bas_max = rng.UniformGenerator(0.00001,0.0001)

mkt = glooper.Market()

agt_gen = glooper.ClassicAgentGenerator(belief,wealth,f_min,p_min,bas_max,mkt,N_agents)

info = rng.UniformGenerator()

agt_pop = glooper.AgentPopulation(info,agt_gen)

sim = glooper.StandardFSSimulation(agt_pop,sim_comment,dbi,simid,N_batches,N_runs,N_steps)
