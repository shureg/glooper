#!/usr/bin/env python

import sys
import os
import os.path
import fnmatch
import optparse
import copy
import re

class DirWalker(object):
   def walk(self,dir,method):
      """ walks a directory, and executes a callback on each file """
      dir = os.path.abspath(dir)
      for file in [file for file in os.listdir(dir) if not file in [".",".."]]:
	 nfile = os.path.join(dir,file)
	 method(nfile)
	 if os.path.isdir(nfile):
	    self.walk(nfile,method)

def readlink_full(link):
   res = link
   while os.path.islink(res):
      res = os.readlink(res)
   return res

def listdir_full(dir):
   files = os.listdir(dir)
   dir = os.path.abspath(dir)
   return map(lambda x: os.path.join(dir,x), files )

parser = optparse.OptionParser()

parser.add_option("--regex",action="store",type="string",dest="regex",
      help="Regular expression to match against the file list")
parser.add_option("-s","--start-code",action="store",type="string",dest="start_code",
      help="The case code of the first file in the list to be processed")
parser.add_option("-e","--end-code",action="store",type="string",dest="end_code",
      help="The case code of the last file in the list to be processed")
parser.add_option("-r","--recursive",action="store_true",dest="recursive",
      help="Traverse the supplied path(s) recursively")
parser.add_option("-o","--logical-or",action="store_false",dest="intersect",
      help="Filter the files that satisfy any of the given conditions")
parser.add_option("-a","--logical-and",action="store_true",dest="intersect",
      help="Filter the files that satisfy all of the given conditions. This is the default")
parser.add_option("--filter-expression",action="store",dest="filter",
      help="A custom Python epxression to be applied as an additional file list filter")
parser.add_option("-n","--dry-run",action="store_true",dest="dry_run",default=False,
      help="Do everything except actually run the simulations [default %default]")

parser.set_defaults(recursive=False)
parser.set_defaults(intersect=True)

(options,args) = parser.parse_args()

if len(args) == 0:
   raise Exception("No arguments given")

dw = DirWalker()

fl_arg = list()

plus_fun = None

if options.intersect:
   plus_fun = lambda y,z: y.intersection(z)
else:
   plus_fun = lambda y,z: y.union(z)

reduce_fun = lambda x: reduce(plus_fun, x)

for p in args:

   lp = copy.deepcopy(p)

   if os.path.islink(lp):
      lp = readlink_full(lp)

   fl = None

   if os.path.isfile(lp):
      
      ff = open(lp,'r')
      fl = set( map( lambda x: os.path.abspath(x.rstrip('\n')), ff.readlines() ) )
      ff.close()
   
   elif os.path.isdir(lp):
      
      if options.recursive:
	 fl = set()
	 dw.walk( lp, lambda x: fl.add(x) )
      else:
	 fl = set(listdir_full(lp))
   
   else:
      raise Exception("Argument %s (%s) is neither a file nor a directory" % (p, lp))

   fl_arg.append(fl)

filenames = reduce_fun(fl_arg)

filenames = map(readlink_full, filenames)

filter_fun = lambda f: filter(f, filenames)

filenames = filter_fun( lambda x: re.search(r"sim\.cfg\.py$",x) )

code_extract = lambda x: x.split("_",2)[0]

if options.regex:
   filenames = plus_fun(set(filenames), filter_fun( lambda x: re.search(options.regex,x)) )

if options.start_code:
   filenames = plus_fun(set(filenames), filter_fun( lambda x: code_extract(os.path.basename(x)) >= options.start_code) )

if options.end_code:
   filenames = plus_fun(set(filenames), filter_fun( lambda x: code_extract(os.path.basename(x)) <= options.end_code) )

if options.filter:
   filenames = plus_fun(set(filenames), filter_fun( eval(options.filter) ) )

filenames = list(filenames)

filenames.sort(key=os.path.basename)

if(options.dry_run):
   print filenames

if(not(options.dry_run)):
   for f in filenames:
      sys.stdout.write("Processing %s\n" % f)
      os.system("./glooper_launcher.py %s" % f)
