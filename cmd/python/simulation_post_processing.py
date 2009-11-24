#!/usr/bin/env python

import sys
import os
import os.path
import logging

#changing the logging level DEBUG label to TRACE to be consistent with the C++ part of the project
logging.addLevelName(10,"TRACE")

xquery_path = "/simulation_data/glooper/lib/xquery/retrieval"
data_path = "/simulation_data/data/db"

def transform_attributes(attrs):
   assert len(attrs) == 4
   filebase = str(attrs[0])
   xquery = str(attrs[1])
   return {
	 "csvfile": os.path.join(data_path,version_string,str(sim_id),"csv","%s.%d.csv" % (filebase, sim_id) ),
	 "xquery_local": os.path.join(data_path,version_string,str(sim_id),"xquery","%s.%d.xquery" % (filebase, sim_id) ),
	 "xquery_source": os.path.join(xquery_path,xquery),


assert len(sys.argv) >= 4, "Not enough arguments supplied (at least 3 needed)"

sim_id = int(sys.argv[1])
version_string = str(sys.argv[3])

log_filename = str(sys.argv[2])
gl_logger = logging.getLogger("simulation_post_processing")
gl_logger.setLevel(logging.DEBUG)
gl_handler = logging.FileHandler(log_filename)
gl_formatter = logging.Formatter("%(asctime)s | <%(name)s> - %(levelname)s: %(message)s")
gl_handler.setFormatter(gl_formatter)
gl_logger.addHandler(gl_handler)

os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id)))
os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id),"xquery"))
os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id),"csv"))

data_attrs = [
      transform_attributes(["agt","agents.ext.xquery"]),
      transform_attributes(["lob","limit_order_book.ext.xquery"]),
      transform_attributes(["trd","trades.ext.xquery"]),
      transform_attributes(["ord","orders.ext.xquery"])
      ]

gl_logger.info("Beginning to process table-compatible data for simulation %d" % sim_id)
for attr in data_attrs:
   gl_logger.info("Creating a simulation-id-specific version of the xquery script %s at %s" % (attr["xquery_source"], attr["xquery_local"]))
   os.system("sed \'s/$1/%d/g\' %s > %s" % (sim_id, attr["xquery_source"], attr["xquery_local"]))
   gl_logger.info("Preparing to execute the query %s abd write the results to csv file %s" % (attr["xquery_local"], attr["xquery_csvfile"]))
   os.system("se_term -file %s -output %s SimulationDB" % (attr["xquery_local"], attr["csv_file"]))

