#!/usr/bin/env python

project_path = "/simulation_data/glooper"

import sys

sys.path.append("%s/lib/python" % project_path)

import os
import string
import sax_top_node_handler_class
import xml.sax
import logging

assert len(sys.argv) >= 6, "Not enough arguments (at least 5 needed)"

xml_filename = str(sys.argv[1])
csv_filename = str(sys.argv[2])

xslt_transform_filename = str(sys.argv[3])
row_node_name = str(sys.argv[4])

parser = xml.sax.make_parser()
handler = sax_top_node_handler_class.SAXFirstNodeHanlder(row_node_name)
parser.setContentHandler(handler)

log_filename = str(sys.argv[5])
gl_logger = logging.getLogger("retrieve_simulation")
gl_logger.setLevel(logging.DEBUG)
gl_handler = logging.FileHandler(log_filename)
gl_formatter = logging.Formatter("%(asctime)s | <%(name)s> - %(levelname)s: %(message)s")
gl_handler.setFormatter(gl_formatter)
gl_logger.addHandler(gl_handler)


gl_logger.info("Beginning xml to csv transformation for a flat xml file %s" % xml_filename)
gl_logger.info("Preparing to parse the first \"row\" for header data in the flat xml file %s" % xml_filename)

try:
   parser.parse(xml_filename)
except sax_top_node_handler_class.SAXExceptionWithData,inst:
   csv_file = open(csv_filename,'w')
   gl_logger.info("Preparing to write header data to csv file %s" % csv_filename)
   csv_file.write(reduce(lambda x,y: x+','+y, inst.data)+'\n')
   csv_file.close()
   gl_logger.info("Preparing to process xml row data in %s with xslt template %s and write csv data to %s" % ( xml_filename,xslt_transform_filename,csv_filename ) )
   os.system("xsltproc -o %s %s %s" % (csv_filename,xslt_transform_filename,xml_filename) )
except xml.sax.SAXException,se:
   logging.log("EXCEPTION",se.getMessage())

gl_logger.info("xml to csv transformation completed (%s -> %s)" % (xml_filename, csv_filename) )
