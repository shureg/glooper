#!/usr/bin/env python

import sys
import string
import os
import os.path

tmpfilename = "/tmp/version_increment.%s.tmp" % str(os.getppid())

assert len(sys.argv) >= 3, "At least two command-line arguments must be provided to \'%s\'\n" % sys.argv[0]
   
version_filename = sys.argv[1]
version_header = sys.argv[2]
version_file = open(version_filename,'r')
version_string = version_file.readline()

if not (os.path.exists(tmpfilename)):

   os.system("touch %s" % tmpfilename)

   version_numbers = string.split(version_string,'.')
   version_file.close()
   
   assert len(version_numbers) == 3, "%s: the version string %s must have exactly three dot-separated parts\n" % (sys.argv[0],version_string)
   
   major = int(version_numbers[0])
   minor = int(version_numbers[1])
   release = int(version_numbers[2])
   
   sys.stdout.write("The automatic version increment system is enabled\n"\
	 "current version number is: %s\n"\
	 "please choose one of the reasons for re-compilation\n"\
	 "1 - Interface change (major number incremented)\n"\
	 "2 - Implementation change (minor number incremented)\n"\
	 "3 - Bugfix (release number incremented)\n"\
	 "0 - Other (version number unchanged)\n" % version_string)
   
   reason = -1
   
   while reason not in [0,1,2,3] :
      reason = int(sys.stdin.readline())

   if reason == 1 :
      major += 1
      minor = 0
      release = 0
   elif reason == 2 :
      minor += 1
      release = 0
   elif reason == 3 :
      release += 1

   version_string = "%s.%s.%s" % (str(major),str(minor),str(release))

   version_file = open(version_filename,'w')
   version_file.write(version_string)
   version_file.close()

   os.system("sed -i \'%s\' %s" % ( "s/LIB_VERSION\\ [0-9]*\\.[0-9]*\\.[0-9]*/LIB_VERSION\\ %s/g" % version_string, version_header ) )
   sys.stdout.write("The new version number is %s\n" % version_string)
