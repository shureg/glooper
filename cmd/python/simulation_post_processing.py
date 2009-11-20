#!/usr/bin/env python

import sys
import os
import os.path
import logging

#changing the logging level DEBUG label to TRACE to be consistent with the C++ part of the project
logging.addLevelName(10,"TRACE")

xquery_path = "/home/shureg/Projects/glooper/lib/xquery/retrieval"
xslt_path = "/home/shureg/Projects/glooper/lib/xslt"
data_path = "/home/shureg/Projects/glooper/data/test"

def transform_attributes(attrs):
   assert len(attrs) == 4
   filebase = str(attrs[0])
   xquery = str(attrs[1])
   xslt = str(attrs[2])
   row_node = str(attrs[3])
   return {
	 "xmlfile": os.path.join(data_path,version_string,str(sim_id),"xml","%s.%d.xml" % (filebase, sim_id) ),
	 "csvfile": os.path.join(data_path,version_string,str(sim_id),"csv","%s.%d.csv" % (filebase, sim_id) ),
	 "xquery": os.path.join(xquery_path,xquery),
	 "xslt": os.path.join(xslt_path,xslt),
	 "row_node": row_node}


assert len(sys.argv) >= 4, "Not enough arguments supplied (at least 3 needed)"

sim_id = int(sys.argv[1])
version_string = str(sys.argv[3])
sim_xml_filename = os.path.join(data_path,version_string,str(sim_id),"raw","sim.%d.xml" % sim_id)
sim_xmldb_name = "tempsimdb"

log_filename = str(sys.argv[2])
gl_logger = logging.getLogger("simulation_post_processing")
gl_logger.setLevel(logging.DEBUG)
gl_handler = logging.FileHandler(log_filename)
gl_formatter = logging.Formatter("%(asctime)s | <%(name)s> - %(levelname)s: %(message)s")
gl_handler.setFormatter(gl_formatter)
gl_logger.addHandler(gl_handler)

os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id)))
os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id),"raw"))
os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id),"csv"))
os.system("mkdir -pv %s" % os.path.join(data_path,version_string,str(sim_id),"xml"))

data_attrs = [
      transform_attributes(["agt","agents.ext.xquery","agents_2csv.xsl","Agent"]),
      transform_attributes(["lob","limit_order_book.ext.xquery","limit_orders_2csv.xsl","LimitOrder"]),
      transform_attributes(["trd","trades.ext.xquery","trades_2csv.xsl","Trade"]),
      transform_attributes(["ord","orders.ext.xquery","orders_2csv.xsl","Order"])
      ]

gl_logger.info("Creating temprory sedna xml database to store simulation %d data" % sim_id)
os.system( "se_term -query \"doc(\\\"SimulationDB\\\")/SimulationData/Simulation[id=%d]\" -output %s SimulationDB" % (sim_id,sim_xml_filename) )
os.system( "se_cdb %s" % sim_xmldb_name )
os.system( "se_sm %s" % sim_xmldb_name )
gl_logger.info("Loading data for simulation %d into the temporary sedna xml database" % sim_id)
os.system( "se_term -query \"LOAD \\\"%s\\\" \\\"%s\\\"\" %s" % (sim_xml_filename,sim_xmldb_name,sim_xmldb_name) )

gl_logger.info("Beginning to process table-compatible data for simulation %d" % sim_id)
for attr in data_attrs:
   gl_logger.info("Extracting data to %s" % attr["xmlfile"])
   os.system( "se_term -file %s -output %s %s" % ( attr["xquery"], attr["xmlfile"], sim_xmldb_name ) )
   gl_logger.info("Transforming data in %s to csv format" % attr["xmlfile"])
   os.system( "env python retrieve_simulation_data.py %s %s %s %s %s" % ( attr["xmlfile"], attr["csvfile"], attr["xslt"], attr["row_node"], log_filename ) )

gl_logger.info("Stopping and deleting the temporary sedna xml database (simulation %d)" % sim_id)
os.system( "se_smsd %s" % sim_xmldb_name)
os.system( "se_ddb %s" % sim_xmldb_name)
