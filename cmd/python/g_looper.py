#!/usr/bin/env python

import sys
import os

for fn in sys.argv[1:]:
   os.system("./glooper_launcher.py %s" % fn)
