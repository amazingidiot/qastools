//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "desktop_items.hpp"

#include "qastools_config.hpp"
#include "info_texts.hpp"
#include "views/view_utility.hpp"

#include <iostream>
#include <getopt.h>


Desktop_Items::Desktop_Items ( )
{
}


Desktop_Items::~Desktop_Items ( )
{
}


int
Desktop_Items::init_settings (
	int argc,
	char * argv[] )
{
	//_dsetup.read_from_storage();
	return parse_cmd_options ( argc, argv );
}


int
Desktop_Items::parse_cmd_options (
	int argc,
	char * argv[] )
{
	// Suppresses error messages by getopt_long
	opterr = 0;

	QString card_idx;
	QString ctl_address;

	bool flag_print_version ( false );
	bool flag_print_help ( false );
	bool flag_print_copy_info ( false );

	bool scan_further ( true );

	while ( scan_further ) {

		static struct option long_opts[] =
		{
			{ "help",    no_argument,  0, 'h' },
			{ "copying", no_argument,  0, 'i' },
			{ "version", no_argument,  0, 'v' },
			{0, 0, 0, 0}
		};

		// getopt_long stores the option index here.
		int long_opts_idx ( 0 );
		int opt_char = getopt_long ( argc, argv, "hc:D:tniv",
			long_opts, &long_opts_idx );

		// Leave loop
		if ( opt_char < 0 ) {
			break;
		}

		switch ( opt_char ) {
			case 0:
				break;
			case 'h':
				flag_print_help = true;
				scan_further = false;
				break;
			case 'i':
				flag_print_copy_info = true;
				break;
			case 'v':
				flag_print_version = true;
				break;
			default:
				// Dont't break, as the option may be for QT
				break;
		}
	}

	if ( flag_print_help ) {
		::std::cout << "Usage:\n";
		::std::cout << "  " << PROGRAM_NAME << " [OPTION]...\n";
		::std::cout << "\n";
		::std::cout << info_text_options;
		::std::cout << "\n";
		::std::cout.flush();
		return -1;
	}

	if ( flag_print_version ) {
		::std::cout << PROGRAM_NAME << " " << VERSION << "\n";
		::std::cout.flush();
		return -1;
	}

	if ( flag_print_copy_info ) {
		::std::cout << PROGRAM_TITLE;
		::std::cout << " - configuration browser for the Linux sound system ALSA.\n";
		::std::cout << "\n";
		::std::cout << license_text_short;
		::std::cout << "\n";
		::std::cout.flush();
		return -1;
	}

	return 0;
}


void
Desktop_Items::start (
	bool )
{
	_mwin.reset ( new Main_Window );

	_mwin->restore_state();
	_mwin->show();
}

