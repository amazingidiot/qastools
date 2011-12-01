#!/usr/bin/python
# -*- coding: utf-8 -*-

import os

script_dir = os.path.dirname(__file__)

pro_file_name = 'app.pro'
pro_file_dir = script_dir + '/../i18n'
pro_file = pro_file_dir + '/' + pro_file_name

src_dirs = [
	script_dir + "/../shared/src",
	script_dir + "/../qasconfig/src",
	script_dir + "/../qashctl/src",
	script_dir + "/../qasmixer/src",
]


hpp_list = []
cpp_list = []
pro_text = ""

# Find header and source files
for src_dir in src_dirs:
	for root, dirs, files in os.walk ( src_dir ):
		for fl in files:
			fl_full = root + "/" + fl
			fl_rel =  os.path.relpath ( fl_full, pro_file_dir )
			if ( fl.endswith ( ".hpp" ) ):
				hpp_list.append ( fl_rel )
			if ( fl.endswith ( ".cpp" ) ):
				cpp_list.append ( fl_rel )

# Headers
hpp_list.sort()
if ( len ( hpp_list ) > 0 ):
	pro_text += "\n"
	pro_text += "HEADERS = \\\n"
	for hpp in hpp_list:
		pro_text += "\t"
		pro_text += hpp
		if ( hpp != hpp_list[-1] ):
			pro_text += " \\\n"
	pro_text += "\n"

# Sources
cpp_list.sort()
if ( len ( cpp_list ) > 0 ):
	pro_text += "\n"
	pro_text += "SOURCES = \\\n"
	for cpp in cpp_list:
		pro_text += "\t"
		pro_text += cpp
		if ( cpp != cpp_list[-1] ):
			pro_text += " \\\n"
	pro_text += "\n"


# Write qastools.pro file
print ( "Writing " + pro_file )
with open ( pro_file, 'w' ) as fl:
	fl.write ( pro_text )
	fl.close()
