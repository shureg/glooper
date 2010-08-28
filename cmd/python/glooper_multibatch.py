#!/usr/bin/env python

import sys
import os
import os.path
import fnmatch

batch_path = str( sys.argv[1] )

filenames = os.listdir(batch_path)

batch_filenames = fnmatch.filter(filenames,"[!.]*.sim.cfg.py")

if len(sys.argv) > 2:
   filter_fun = eval(sys.argv[2])
   batch_filenames = filter(filter_fun,batch_filenames)

batch_filenames.sort()

batch_cfg_files = map(lambda x: os.path.join(batch_path,x), batch_filenames )

for f in batch_cfg_files:
   os.system("./glooper_launcher.py %s" % f)
