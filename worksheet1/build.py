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
parser.add_argument('--document', '-d', help='Generate LaTex documentation for the project as well.', action='store_true')
parser.add_argument('--only-document', '-D', help='Only generate LaTex documentation.', action='store_true')

compiler="g++"
compiler_flags=["-Wall", "-O2"]
source_dir="source/"
object_dir="images/"
document_dir="documentation/"
tex_name="test.tex"

executables_to_compile={"w1q2.cpp" : "hello",
        "w1q3a.cpp" : "maxint",
        "w1q3b.cpp" : "maxfloat",
        "w1q3c.cpp" : "difference",
        "w1q3d.cpp" : "sums",
        "w1q4.cpp" : "silverratio",
        "w1q6.cpp" : "bisector",
        "w1q7.cpp" : "newton-raphson"}

print "Beginning build."

args = parser.parse_args()

print args

if args.only_document == True:
    os.chdir(document_dir)
    call(['pdflatex',  tex_name])
    os.chdir('..')

elif args.sources==None:
    
    for source_file in executables_to_compile:
        exe_list = [compiler, "-o", object_dir+executables_to_compile[source_file], source_dir+source_file]
        if args.suppress_flags==False:
            exe_list[1:1] = compiler_flags 
        if not args.flags==None:
            for item in args.flags:
                exe_list.insert(1, "-"+item)
        print exe_list
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

if args.document == True:
    os.chdir(document_dir)
    call(['pdflatex', document_dir])
    os.chdir('..')

#def buildlatex():
#    os.chdir(document_dir)
#    call(['pdflatex', document_dir])
#    call(['bibtext', tex_name[:-4])
#    call(['pdflatex', document_dir])
#    call(['pdflatex', document_dir])
#    os.chdir('..')
