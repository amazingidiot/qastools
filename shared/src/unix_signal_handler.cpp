/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "unix_signal_handler.hpp"
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <QSocketNotifier>
#include <iostream>

int Unix_Signal_Handler::_sig_int_fds[ 2 ];
int Unix_Signal_Handler::_sig_hup_fds[ 2 ];
int Unix_Signal_Handler::_sig_term_fds[ 2 ];

Unix_Signal_Handler::Unix_Signal_Handler ( QObject * parent_n )
: QObject ( parent_n )
{
  if ( ::socketpair ( AF_UNIX, SOCK_STREAM, 0, _sig_int_fds ) ) {
    qFatal ( "Couldn't create SIGINT socketpair" );
  }

  if ( ::socketpair ( AF_UNIX, SOCK_STREAM, 0, _sig_hup_fds ) ) {
    qFatal ( "Couldn't create SIGHUP socketpair" );
  }

  if ( ::socketpair ( AF_UNIX, SOCK_STREAM, 0, _sig_term_fds ) ) {
    qFatal ( "Couldn't create SIGTERM socketpair" );
  }

  _sn_int.reset (
      new QSocketNotifier ( _sig_int_fds[ 1 ], QSocketNotifier::Read, this ) );

  _sn_hup.reset (
      new QSocketNotifier ( _sig_hup_fds[ 1 ], QSocketNotifier::Read, this ) );

  _sn_term.reset (
      new QSocketNotifier ( _sig_term_fds[ 1 ], QSocketNotifier::Read, this ) );

  connect ( _sn_int.get (),
            SIGNAL ( activated ( int ) ),
            this,
            SLOT ( sev_handle_sig_int () ) );

  connect ( _sn_hup.get (),
            SIGNAL ( activated ( int ) ),
            this,
            SLOT ( sev_handle_sig_hup () ) );

  connect ( _sn_term.get (),
            SIGNAL ( activated ( int ) ),
            this,
            SLOT ( sev_handle_sig_term () ) );
}

Unix_Signal_Handler::~Unix_Signal_Handler () {}

int
Unix_Signal_Handler::init_unix_signal_handlers ()
{
  {
    struct sigaction act;
    act.sa_handler = Unix_Signal_Handler::signal_handler_int;
    sigemptyset ( &act.sa_mask );
    act.sa_flags = 0;
    act.sa_flags |= SA_RESTART;

    if ( sigaction ( SIGINT, &act, 0 ) > 0 ) {
      return 1;
    }
  }

  {
    struct sigaction act;
    act.sa_handler = Unix_Signal_Handler::signal_handler_hup;
    sigemptyset ( &act.sa_mask );
    act.sa_flags = 0;
    act.sa_flags |= SA_RESTART;

    if ( sigaction ( SIGHUP, &act, 0 ) > 0 ) {
      return 2;
    }
  }

  {
    struct sigaction act;
    act.sa_handler = Unix_Signal_Handler::signal_handler_term;
    sigemptyset ( &act.sa_mask );
    act.sa_flags = 0;
    act.sa_flags |= SA_RESTART;

    if ( sigaction ( SIGTERM, &act, 0 ) > 0 ) {
      return 3;
    }
  }

  return 0;
}

void
Unix_Signal_Handler::signal_handler_int ( int )
{
  char dat ( 1 );
  int size ( sizeof ( dat ) );
  if ( ::write ( _sig_int_fds[ 0 ], &dat, size ) != size ) {
    qFatal ( "Unix_Signal_Handler: Writing to SIGINT socket failed" );
  }
}

void
Unix_Signal_Handler::signal_handler_hup ( int )
{
  char dat ( 1 );
  int size ( sizeof ( dat ) );
  if ( ::write ( _sig_hup_fds[ 0 ], &dat, size ) != size ) {
    qFatal ( "Unix_Signal_Handler: Writing to SIGHUP socket failed" );
  }
}

void
Unix_Signal_Handler::signal_handler_term ( int )
{
  char dat ( 1 );
  int size ( sizeof ( dat ) );
  if ( ::write ( _sig_term_fds[ 0 ], &dat, size ) != size ) {
    qFatal ( "Unix_Signal_Handler: Writing to SIGTERM socket failed" );
  }
}

void
Unix_Signal_Handler::sev_handle_sig_int ()
{
  //::std::cout << "Unix_Signal_Handler::sev_handle_sig_int\n";

  _sn_int->setEnabled ( false );
  char tmp;
  int size ( sizeof ( tmp ) );
  if ( ::read ( _sig_int_fds[ 1 ], &tmp, size ) == -1 ) {
    qFatal ( "Unix_Signal_Handler: Reading from SIGINT socket failed" );
  }

  emit sig_int ();

  _sn_int->setEnabled ( true );
}

void
Unix_Signal_Handler::sev_handle_sig_hup ()
{
  //::std::cout << "Unix_Signal_Handler::sev_handle_sig_hup\n";

  _sn_hup->setEnabled ( false );
  char tmp;
  int size ( sizeof ( tmp ) );
  if ( ::read ( _sig_hup_fds[ 1 ], &tmp, size ) == -1 ) {
    qFatal ( "Unix_Signal_Handler: Reading from SIGHUP socket failed" );
  }

  emit sig_hup ();

  _sn_hup->setEnabled ( true );
}

void
Unix_Signal_Handler::sev_handle_sig_term ()
{
  //::std::cout << "Unix_Signal_Handler::sev_handle_sig_term\n";

  _sn_term->setEnabled ( false );
  char tmp;
  int size ( sizeof ( tmp ) );
  if ( ::read ( _sig_term_fds[ 1 ], &tmp, size ) == -1 ) {
    qFatal ( "Unix_Signal_Handler: Reading from SIGTERM socket failed" );
  }

  emit sig_term ();

  _sn_term->setEnabled ( true );
}
