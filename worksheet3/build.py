#!/usr/bin/python

# Python script to build files in worksheet1 to source.
#
# make is an awful language and I refuse to use it. cmake is no better.
# 


import argparse
from subprocess import call
import os

parser = argparse.ArgumentParser(description='Build automater written by Mike Knee')

parser.add_argument('--flags', '-f', nargs='*', help='Specify non-default compiler flags. The "-" should be omitted.')
parser.add_argument('--suppress-flags', help='Suppress default compiler switches', action='store_true')
parser.add_argument('--sources', '-s', nargs='*', help='List of specific sources to compile.')

compiler="g++"
compiler_flags=["-Wall", "-O2", "-I/usr/include", "-lgsl", "-lgslcblas", "-lm"]
source_dir="source/"
object_dir="images/"

executables_to_compile={"question1.cpp" : "euler",
			"question5.cpp" : "rungekutta",
			"question5-2.cpp" : "gsl_rk"}

print
print "Beginning build."
print

args = parser.parse_args()

if args.sources==None:
    
    for source_file in executables_to_compile:
        exe_list = [compiler, "-o", object_dir+executables_to_compile[source_file], source_dir+source_file]
        if args.suppress_flags==False:
            exe_list[1:1] = compiler_flags 
        if not args.flags==None:
            for item in args.flags:
                exe_list.insert(1, "-"+item)
        print "Calling: " + ' '.join(exe_list)
	print
        call(exe_list)

else:

    for item in args.sources:
        exe_list = [compiler, "-o", object_dir+executables_to_compile[item], source_dir+item]
        if args.suppress_flags==False:
            exe_list[1:1] = compiler_flags 
        if not args.flags==None:
            for item in args.flags:
                exe_list.insert(1, "-"+item)
        print exe_list
        call(exe_list)

print
print "Done!"
print "Hooray!"
print
