#!/usr/bin/env python
import sys
import os
import os.path
import logging

#changing the logging level DEBUG label to TRACE to be consistent with the C++ part of the project
logging.addLevelName(10,"TRACE")

gl_logger = logging.getLogger("glooper_launcher")

glooper_bin_path = "/simulation_data/glooper/cmd/c++/glooper"
log_path ="/simulation_data/data/log"
post_processing_script = "simulation_post_processing.py"

if len(sys.argv) < 4:
   sys.stderr.write("usage: [python] glooper_launcher.py {executable_file} {simulation_id_tracking_file} {version_number_file}\n")
   sys.exit()

glooper_executable = os.path.join(glooper_bin_path,str(sys.argv[1]))
simid_filename = str(sys.argv[2])
version_filename = str(sys.argv[3])

assert os.path.exists(glooper_executable), "File %s does not exist" % glooper_executable
assert os.path.exists(simid_filename), "File %s does not exist" % simid_filename
assert os.path.exists(version_filename), "File %s does not exist" % version_filename

simid_file = open(simid_filename,'r')
simid = int( simid_file.readline() )
simid_file.close()

simid += 1

simid_file = open(simid_filename, 'w')
simid_file.write(str(simid))
simid_file.close()

version_file = open(version_filename, 'r')
version_string = str(version_file.readline())
version_file.close()

log_filename = os.path.join(log_path,version_string,str(simid),"sim.%d.log" % simid )
os.system("mkdir -pv %s" % os.path.join(log_path,version_string,str(simid)))

gl_logger.setLevel(logging.DEBUG)
gl_handler = logging.FileHandler(log_filename)
gl_formatter = logging.Formatter("%(asctime)s | <%(name)s> - %(levelname)s: %(message)s")
gl_handler.setFormatter(gl_formatter)
gl_logger.addHandler(gl_handler)

gl_logger.info("Preparing to start simulation %d" % simid)
os.system( "%s %d %s" % ( glooper_executable, simid, log_filename) )

gl_logger.info("Simulation %d completed, starting post-processing of simulation data" % simid)
os.system( "env python %s %d %s %s" % ( post_processing_script, simid, log_filename, version_string ) )
gl_logger.info("Post-processing for simulation %d complete - exiting" % simid)
