#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import re
import sys
import shutil

script_dir = os.path.dirname ( __file__ )

src_dirs = [
	script_dir + "/../shared/src",
	script_dir + "/../qasconfig/src",
	script_dir + "/../qashctl/src",
	script_dir + "/../qasmixer/src",
]

cmd_arg_error = True
do_list_files = False
do_list_files_inverted = False
seriously_do_it = False

if ( len ( sys.argv ) > 1 ):
	if ( len ( sys.argv ) > 2 ):
		print "Too many arguments"
	else:
		cmd_arg_error = False
		if ( sys.argv[1] == '--help' ):
			print '''\
Options:
	--help  - Print a help text
	--list  - List matching files
	--list-invert - List not matching files
	--do-it - Actually change file headers\
'''
		elif ( sys.argv[1] == '--list' ):
			do_list_files = True
		elif ( sys.argv[1] == '--list-invert' ):
			do_list_files_inverted = True
		elif ( sys.argv[1] == '--do-it' ):
			seriously_do_it = True
		else:
			print ( "Unknown argument: " + sys.argv[1] )
			print ( "Try --help" )
			cmd_arg_error = True
			exit ( -1 )

if ( cmd_arg_error ):
	print ( "Try --help" )
	exit ( -1 )

file_list = []

# Find files in the source directories
for src_dir in src_dirs:
	for root, dirs, files in os.walk ( src_dir ):
		for fl in files:
			fl_full = root + "/" + fl
			fl_rel =  os.path.relpath ( fl_full, script_dir )
			file_list.append ( fl_rel )
file_list.sort()

# Source file name filter
filter_regs = [
	re.compile ( ".*\.hpp$" ),
	re.compile ( ".*\.cpp$" )
]


file_list_filtered = []

for fname in file_list:
	#print fname
	for fl in filter_regs:
		if ( fl.match ( fname ) ):
			file_list_filtered.append ( fname )

hrepl_in = '''\
/// QasTools: Dektop toolset for the Linux sound system ALSA.
/// \\\\copyright See COPYING file.\
'''

hrepl_out = '''\
/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.\
'''

hrepl_in = hrepl_in.replace ( '/', '\/' )
hrepl_in = hrepl_in.replace ( '(', '\(' )
hrepl_in = hrepl_in.replace ( ')', '\)' )
hrepl_in = hrepl_in.replace ( '+', '\+' )

#print hrepl_in
re_repl = re.compile ( hrepl_in )


tmp_file = "/tmp/header_fix.txt"
for fname in file_list_filtered:
	with open ( fname, 'r' ) as fhandle:
		fdata = fhandle.read()
	if ( re_repl.match ( fdata ) ):
		if ( do_list_files ):
			print fname

		if ( seriously_do_it ):
			print fname
			fdata_fixed = re_repl.sub ( hrepl_out, fdata )
			with open ( tmp_file, 'w' ) as fhandle:
				fhandle.write ( fdata_fixed )
			shutil.move ( tmp_file, fname )
	else:
		if ( do_list_files_inverted ):
			print fname
