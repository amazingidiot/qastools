/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_info_db.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <iostream>
#include <sstream>

namespace QSnd
{

Mixer_HCTL::Mixer_HCTL ( QObject * parent_n )
: QObject ( parent_n )
, _snd_hctl ( 0 )
, _iface_num_types ( 0 )
, _update_requested ( false )
, _reload_requested ( false )
{
  _info_db = new ::QSnd::Mixer_HCTL_Info_Db;

  for ( unsigned int ii = 0; ii < info_db ()->num_iface_types (); ++ii ) {
    _iface_avail_types[ ii ] = 0;
  }
  for ( unsigned int ii = 0; ii < info_db ()->num_iface_types (); ++ii ) {
    _iface_type_count[ ii ] = 0;
  }
}

Mixer_HCTL::~Mixer_HCTL ()
{
  close ();
  delete _info_db;
}

void
Mixer_HCTL::close ()
{
  // Destroy socket notifiers
  if ( _socket_notifiers.size () > 0 ) {
    for ( int ii = 0; ii < _socket_notifiers.size (); ++ii ) {
      delete _socket_notifiers[ ii ];
    }
    _socket_notifiers.clear ();
    _pollfds.clear ();
  }

  if ( _mixer_elems.size () > 0 ) {
    // Disconnect mixer elements
    for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
      snd_hctl_elem_t * elem ( _mixer_elems[ ii ]->snd_hctl_elem () );
      snd_hctl_elem_set_callback ( elem, 0 );
      snd_hctl_elem_set_callback_private ( elem, 0 );
    }

    // Destroy mixer elements
    for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
      delete _mixer_elems[ ii ];
    }
    _mixer_elems.clear ();
  }

  if ( snd_hctl () != 0 ) {
    snd_hctl_set_callback ( snd_hctl (), 0 );
    snd_hctl_set_callback_private ( snd_hctl (), 0 );

    snd_hctl_close ( snd_hctl () );
    _snd_hctl = 0;
  }

  // Clear statistics
  _iface_num_types = 0;
  for ( unsigned int ii = 0; ii < info_db ()->num_iface_types (); ++ii ) {
    _iface_avail_types[ ii ] = 0;
  }
  for ( unsigned int ii = 0; ii < info_db ()->num_iface_types (); ++ii ) {
    _iface_type_count[ ii ] = 0;
  }
}

int
Mixer_HCTL::open ( const QString & dev_name_n )
{
  close ();

  _err_func.clear ();
  _err_message.clear ();

  if ( dev_name_n.isEmpty () ) {
    _err_func = "open";
    _err_message = tr ( "Empty device name" );
    return -1;
  }

  // Open control handle
  int err;
  err = snd_hctl_open (
      &_snd_hctl, dev_name_n.toLocal8Bit ().constData (), SND_CTL_NONBLOCK );

  if ( err < 0 ) {
    _err_func = "snd_hctl_open";
    _err_message = snd_error_qstring ( err );
    close ();
    return err;
  }

  // Load mixer
  err = snd_hctl_load ( snd_hctl () );
  if ( err < 0 ) {
    _err_func = "snd_hctl_load";
    _err_message = snd_error_qstring ( err );
    close ();
    return err;
  }

  snd_hctl_set_callback ( snd_hctl (), &Mixer_HCTL::alsa_callback_hctl );
  snd_hctl_set_callback_private ( snd_hctl (), this );

  // Create mixer elements
  err = create_mixer_elems ();
  if ( err < 0 ) {
    close ();
    return err;
  }

  // Create socket notifiers
  err = create_socket_notifiers ();
  if ( err < 0 ) {
    close ();
    return err;
  }

  // Update statistics
  for ( unsigned int ii = 0; ii < num_elems (); ++ii ) {
    Mixer_HCTL_Elem * snd_elem ( elem ( ii ) );
    unsigned int type_idx ( info_db ()->iface_type_idx ( snd_elem->iface () ) );
    _iface_type_count[ type_idx ] += 1;
  }

  _iface_num_types = 0;
  for ( unsigned int ii = 0; ii < info_db ()->num_iface_types (); ++ii ) {
    if ( _iface_type_count[ ii ] > 0 ) {
      _iface_avail_types[ _iface_num_types ] = ii;
      ++_iface_num_types;
    }
  }

  return 0;
}

int
Mixer_HCTL::create_mixer_elems ()
{
  // Create and connect Mixer_HCTL_Elem items
  snd_hctl_elem_t * hctl_elem;

  hctl_elem = snd_hctl_first_elem ( snd_hctl () );
  while ( hctl_elem != 0 ) {
    Mixer_HCTL_Elem * smce =
        new Mixer_HCTL_Elem ( *info_db (), this, snd_hctl (), hctl_elem );

    snd_hctl_elem_set_callback_private ( hctl_elem, smce );
    snd_hctl_elem_set_callback ( hctl_elem,
                                 &Mixer_HCTL_Elem::alsa_callback_hctl_elem );

    _mixer_elems.append ( smce );

    hctl_elem = snd_hctl_elem_next ( hctl_elem );
  }

  return 0;
}

int
Mixer_HCTL::create_socket_notifiers ()
{
  //::std::cout << "Mixer_HCTL::create_socket_notifiers" << "\n";

  // Setup socket notifiers
  const int num_fds ( snd_hctl_poll_descriptors_count ( snd_hctl () ) );

  if ( num_fds <= 0 ) {
    _err_func = "snd_hctl_poll_descriptors_count";
    _err_message = snd_error_qstring ( num_fds );
    return num_fds;
  }

  _pollfds.resize ( num_fds );
  memset ( &_pollfds[ 0 ], 0, num_fds * sizeof ( pollfd ) );

  int err = snd_hctl_poll_descriptors ( snd_hctl (), &_pollfds[ 0 ], num_fds );
  if ( err < 0 ) {
    _err_func = "snd_hctl_poll_descriptors";
    _err_message = snd_error_qstring ( err );
    return err;
  }

  for ( int ii = 0; ii < num_fds; ++ii ) {
    const int fd ( _pollfds[ ii ].fd );
    if ( fd != 0 ) {
      QSocketNotifier * snot (
          new QSocketNotifier ( fd, QSocketNotifier::Read, this ) );

      connect ( snot,
                SIGNAL ( activated ( int ) ),
                this,
                SLOT ( socket_event ( int ) ) );

      _socket_notifiers.append ( snot );
    }
  }

  return 0;
}

void
Mixer_HCTL::set_socked_notifiers_enabled ( bool flag_n )
{
  for ( int ii = 0; ii < _socket_notifiers.size (); ++ii ) {
    _socket_notifiers[ ii ]->setEnabled ( flag_n );
  }
}

void
Mixer_HCTL::signalize_all_changes ()
{
  for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
    _mixer_elems[ ii ]->signalize_changes ();
  }
}

bool
Mixer_HCTL::event ( QEvent * event_n )
{
  bool res ( true );

  if ( event_n->type () == ::QSnd::evt_update_values_request ) {

    if ( !_update_requested ) {
      _update_requested = true;
      QCoreApplication::postEvent ( this,
                                    new QEvent ( ::QSnd::evt_update_values ) );
    }

  } else if ( event_n->type () == ::QSnd::evt_update_values ) {

    _update_requested = false;
    signalize_all_changes ();

  } else if ( event_n->type () == ::QSnd::evt_reload_request ) {

    if ( !_reload_requested ) {
      _reload_requested = true;
      QCoreApplication::postEvent ( this, new QEvent ( ::QSnd::evt_reload ) );
    }

  } else if ( event_n->type () == ::QSnd::evt_reload ) {

    _reload_requested = false;
    emit sig_mixer_reload_request ();

  } else {
    res = QObject::event ( event_n );
  }

  return res;
}

void
Mixer_HCTL::socket_event ( int )
{
  if ( snd_hctl () != 0 ) {

    int num_ev ( snd_hctl_handle_events ( snd_hctl () ) );
    if ( num_ev < 0 ) {
      // Error. Disable socket notifiers
      set_socked_notifiers_enabled ( false );
      print_alsa_error ( "snd_hctl_handle_events ", num_ev );
    }

    signalize_all_changes ();
  }
}

void
Mixer_HCTL::request_reload ()
{
  QCoreApplication::postEvent ( this,
                                new QEvent ( ::QSnd::evt_reload_request ) );
}

// Alsa callbacks

int
Mixer_HCTL::alsa_callback_hctl ( snd_hctl_t * snd_hctl_n,
                                 unsigned int mask_n,
                                 snd_hctl_elem_t * )
{
  int res ( 0 );

  Mixer_HCTL * qsnd_mixer;
  {
    void * priv ( snd_hctl_get_callback_private ( snd_hctl_n ) );
    qsnd_mixer = reinterpret_cast< Mixer_HCTL * > ( priv );
  }

  if ( qsnd_mixer != 0 ) {
    const unsigned int change_mask (
        SND_CTL_EVENT_MASK_VALUE | SND_CTL_EVENT_MASK_INFO |
        SND_CTL_EVENT_MASK_ADD | SND_CTL_EVENT_MASK_TLV );

    if ( ( mask_n == SND_CTL_EVENT_MASK_REMOVE ) ||
         ( ( mask_n & change_mask ) != 0 ) ) {
      qsnd_mixer->request_reload ();
    } else {
      {
        ::std::stringstream msg;
        msg << "Mixer_HCTL::alsa_callback_hctl: ";
        msg << "Unknown mask ( " << mask_n << " )" << ::std::endl;
        ::std::cerr << msg.str ();
      }
      res = -1;
    }
  }

  return res;
}

} // namespace QSnd
