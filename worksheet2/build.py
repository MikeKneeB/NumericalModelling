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
compiler_flags=["-Wall", "-O2", "-I/usr/include", "-lgsl", "-lgslcblas", "-lm"]
source_dir="source/"
object_dir="images/"
document_dir="documentation/"
tex_name="worksheet2.tex"

executables_to_compile={"question1.cpp" : "trapezium",
        "question2.cpp" : "ext_trapezium",
        "question6.cpp" : "gsl_1",
	"question7.cpp" : "gsl_2"}

def buildlatex():
    os.chdir(document_dir)
    call(['pdflatex', tex_name])
    call(['bibtex', tex_name[:-4]])
    call(['pdflatex', tex_name])
    call(['pdflatex', tex_name])
    os.chdir('..')

print "Beginning build."

args = parser.parse_args()

print args

if args.only_document == True:
    buildlatex()

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
    if args.document == True:
        buildlatex()

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
    if agrs.document == True:
        buildlatex()


