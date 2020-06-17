/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_unix_signal_handler_hpp__
#define __INC_unix_signal_handler_hpp__

#include <QObject>
#include <memory>

// Forward declaration
class QSocketNotifier;

class Unix_Signal_Handler : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Unix_Signal_Handler ( QObject * parent_n );

  ~Unix_Signal_Handler ();

  static int
  init_unix_signal_handlers ();

  // Unix signal handlers.

  static void
  signal_handler_int ( int unused_n );

  static void
  signal_handler_hup ( int unused_n );

  static void
  signal_handler_term ( int unused_n );

  // Signals
  signals:

  void
  sig_int ();

  void
  sig_hup ();

  void
  sig_term ();

  // Public slots
  public slots:

  void
  sev_handle_sig_int ();

  void
  sev_handle_sig_hup ();

  void
  sev_handle_sig_term ();

  // Private attributes
  private:
  static int _sig_int_fds[ 2 ];
  static int _sig_hup_fds[ 2 ];
  static int _sig_term_fds[ 2 ];

  ::std::unique_ptr< QSocketNotifier > _sn_int;
  ::std::unique_ptr< QSocketNotifier > _sn_hup;
  ::std::unique_ptr< QSocketNotifier > _sn_term;
};

#endif
