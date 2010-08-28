#!/usr/bin/env python

import os
import os.path
import sys

outdir = str(sys.argv[2])

os.system("rm -rf %s" % outdir);

tbl_id = ["agt","trd","ord","inf"]

for x in tbl_id:
   os.system("mkdir -p %s/%s" % (outdir, x) )

os.system("./table_db %s %s" % (sys.argv[1], outdir))
