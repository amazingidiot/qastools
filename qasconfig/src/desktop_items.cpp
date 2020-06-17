/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "desktop_items.hpp"
#include "info_texts.hpp"
#include "qastools_config.hpp"
#include <getopt.h>
#include <iostream>
#include <sstream>

Desktop_Items::Desktop_Items () {}

Desktop_Items::~Desktop_Items () {}

int
Desktop_Items::init_settings ( int argc, char * argv[] )
{
  //_dsetup.read_from_storage();
  return parse_cmd_options ( argc, argv );
}

int
Desktop_Items::parse_cmd_options ( int argc, char * argv[] )
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

    static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                        {"copying", no_argument, 0, 'i'},
                                        {"version", no_argument, 0, 'v'},
                                        {0, 0, 0, 0}};

    // getopt_long stores the option index here.
    int long_opts_idx ( 0 );
    int opt_char =
        getopt_long ( argc, argv, "hc:D:tniv", long_opts, &long_opts_idx );

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
    {
      ::std::stringstream msg;
      msg << "Usage:" << ::std::endl;
      msg << "  " << PROGRAM_NAME << " [OPTION]..." << ::std::endl;
      msg << ::std::endl;
      msg << info_text_options;
      msg << ::std::endl;
      ::std::cout << msg.str () << ::std::flush;
    }
    return -1;
  }

  if ( flag_print_version ) {
    {
      ::std::stringstream msg;
      msg << PROGRAM_NAME << " " << VERSION << ::std::endl;
      ::std::cout << msg.str () << ::std::flush;
    }
    return -1;
  }

  if ( flag_print_copy_info ) {
    {
      ::std::stringstream msg;
      msg << PROGRAM_TITLE;
      msg << " - configuration browser for the Linux sound system ALSA.";
      msg << ::std::endl;
      msg << ::std::endl;
      msg << license_text_short;
      msg << ::std::endl;
      ::std::cout << msg.str () << ::std::flush;
    }
    return -1;
  }

  return 0;
}

void
Desktop_Items::start ( bool )
{
  _mwin.reset ( new Main_Window );

  _mwin->restore_state ();
  _mwin->show ();
}
