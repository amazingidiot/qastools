/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_simple.hpp"
#include "qsnd/event_types.hpp"
#include "qsnd/mixer_simple_elem.hpp"
#include "qsnd/mixer_simple_filter.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <iostream>
#include <sstream>

namespace QSnd
{

Mixer_Simple::Mixer_Simple ( QObject * parent_n )
: QObject ( parent_n )
, _snd_hctl ( 0 )
, _snd_mixer ( 0 )
, _update_requested ( false )
, _reload_requested ( false )
{
  _num_elems_stream[ 0 ] = 0;
  _num_elems_stream[ 1 ] = 0;
  _num_elems_volume[ 0 ] = 0;
  _num_elems_volume[ 1 ] = 0;
  _num_elems_no_volume[ 0 ] = 0;
  _num_elems_no_volume[ 1 ] = 0;
}

Mixer_Simple::~Mixer_Simple ()
{
  close ();
  clear_elem_filters ();
}

void
Mixer_Simple::clear_elem_filters ()
{
  for ( int ii = 0; ii < _elem_filters.size (); ++ii ) {
    delete _elem_filters[ ii ];
  }
  _elem_filters.clear ();
}

void
Mixer_Simple::append_elem_filter ( ::QSnd::Mixer_Simple_Filter * filter_n )
{
  if ( filter_n != 0 ) {
    _elem_filters.append ( filter_n );
  }
}

void
Mixer_Simple::close ()
{

  // Destroy socket notifiers
  for ( int ii = 0; ii < _socket_notifiers.size (); ++ii ) {
    delete _socket_notifiers[ ii ];
  }
  _socket_notifiers.clear ();
  _pollfds.clear ();

  clear_mixer_elems ();

  if ( snd_mixer () != 0 ) {
    snd_mixer_set_callback ( snd_mixer (), 0 );
    snd_mixer_set_callback_private ( snd_mixer (), 0 );

    if ( _snd_hctl != 0 ) {
      snd_mixer_detach_hctl ( snd_mixer (), _snd_hctl );
    }

    snd_mixer_close ( snd_mixer () );
    _snd_mixer = 0;
  }

  if ( _snd_hctl != 0 ) {
    snd_hctl_close ( _snd_hctl );
    _snd_hctl = 0;
  }

  _num_elems_stream[ 0 ] = 0;
  _num_elems_stream[ 1 ] = 0;
  _num_elems_volume[ 0 ] = 0;
  _num_elems_volume[ 1 ] = 0;
  _num_elems_no_volume[ 0 ] = 0;
  _num_elems_no_volume[ 1 ] = 0;
}

int
Mixer_Simple::open ( const QString & dev_name_n )
{
  //::std::cout << "Mixer_Simple::open " << dev_name_n.toLocal8Bit().constData()
  //<< "\n";

  close ();

  _err_func.clear ();
  _err_message.clear ();

  if ( dev_name_n.isEmpty () ) {
    _err_func = "open";
    _err_message = tr ( "Empty device name" );
    return -1;
  }

  int err;
  err = snd_mixer_open ( &_snd_mixer, 0 );
  if ( err < 0 ) {
    _err_func = "snd_mixer_open";
    _err_message = snd_error_qstring ( err );
    return err;
  }

  err = snd_mixer_selem_register ( _snd_mixer, NULL, NULL );
  if ( err < 0 ) {
    _err_func = "snd_mixer_selem_register";
    _err_message = snd_error_qstring ( err );
    close ();
    return err;
  }

  // Open control handle
  err = snd_hctl_open (
      &_snd_hctl, dev_name_n.toLocal8Bit ().constData (), SND_CTL_NONBLOCK );

  if ( err < 0 ) {
    _err_func = "snd_hctl_open";
    _err_message = snd_error_qstring ( err );
    close ();
    return err;
  }

  // Attach hctl handle to mixer
  err = snd_mixer_attach_hctl ( _snd_mixer, _snd_hctl );
  if ( err < 0 ) {
    _snd_hctl = 0;
    _err_func = "snd_mixer_attach_hctl";
    _err_message = snd_error_qstring ( err );
    close ();
    return err;
  }

  // Load mixer
  err = snd_mixer_load ( _snd_mixer );
  if ( err < 0 ) {
    _err_func = "snd_mixer_load";
    _err_message = snd_error_qstring ( err );
    close ();
    return err;
  }

  // Install alsa callback
  snd_mixer_set_callback_private ( snd_mixer (), this );
  snd_mixer_set_callback ( snd_mixer (),
                           &::QSnd::Mixer_Simple::alsa_callback_mixer );

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

  return 0;
}

void
Mixer_Simple::clear_mixer_elems ()
{
  if ( _mixer_elems.size () > 0 ) {
    // Disconnect mixer elements
    for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
      destroy_mixer_elem ( _mixer_elems[ ii ] );
    }
    _mixer_elems.clear ();
  }
}

int
Mixer_Simple::create_mixer_elems ()
{
  // Create and Mixer_Simple_Elem objects
  {
    snd_mixer_elem_t * elem;
    elem = snd_mixer_first_elem ( snd_mixer () );
    while ( elem != 0 ) {
      {
        Mixer_Simple_Elem * mx_elem ( create_mixer_elem ( elem ) );
        _mixer_elems.append ( mx_elem );
      }
      elem = snd_mixer_elem_next ( elem );
    }
  }

  // Filter Mixer_Simple_Elem objects
  if ( _elem_filters.size () > 0 ) {
    // List for dropped (filtered) elements
    QList< Mixer_Simple_Elem * > elems_dropped;

    for ( int ii = 0; ii < _elem_filters.size (); ++ii ) {
      _elem_filters[ ii ]->filter ( _mixer_elems, elems_dropped );
    }

    // Destroy dropped elements
    for ( int ii = 0; ii < elems_dropped.size (); ++ii ) {
      destroy_mixer_elem ( elems_dropped[ ii ] );
    }
  }

  // Handle names that occur multiple times
  const QString str_new ( "%1, %2" );
  for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
    Mixer_Simple_Elem * mx_elem ( _mixer_elems[ ii ] );
    QString str_cur ( mx_elem->display_name () );

    bool dbl ( false );

    // Detect if there're doubles at all
    for ( int jj = ii + 1; jj < _mixer_elems.size (); ++jj ) {
      Mixer_Simple_Elem * mx_elem2 ( _mixer_elems[ jj ] );
      if ( mx_elem2->display_name () == str_cur ) {
        dbl = true;
        break;
      }
    }

    // Rename doubles
    if ( dbl ) {
      for ( int jj = ii; jj < _mixer_elems.size (); ++jj ) {
        Mixer_Simple_Elem * mx_elem2 ( _mixer_elems[ jj ] );
        if ( mx_elem2->display_name () == str_cur ) {
          QString nname ( str_new );
          nname = nname.arg ( str_cur );
          nname = nname.arg ( mx_elem2->elem_index () );
          mx_elem2->set_display_name ( nname );
        }
      }
    }
  }

  // Acquire some statistics
  for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
    Mixer_Simple_Elem * mx_elem ( _mixer_elems[ ii ] );
    for ( unsigned int sdir = 0; sdir < 2; ++sdir ) {

      bool has_vol ( mx_elem->has_volume ( sdir ) );
      bool has_switch ( mx_elem->has_switch ( sdir ) );
      bool has_enum ( mx_elem->has_enum ( sdir ) );

      if ( has_vol ) {
        ++_num_elems_volume[ sdir ];
      } else {
        if ( has_switch || has_enum ) {
          ++_num_elems_no_volume[ sdir ];
        }
      }
      if ( has_vol || has_switch || has_enum ) {
        ++_num_elems_stream[ sdir ];
      }
    }
  }

  return 0;
}

void
Mixer_Simple::destroy_mixer_elem ( Mixer_Simple_Elem * elem_n )
{
  snd_mixer_elem_t * snd_elem ( elem_n->snd_mixer_elem () );
  snd_mixer_elem_set_callback ( snd_elem, 0 );
  snd_mixer_elem_set_callback_private ( snd_elem, 0 );
  delete elem_n;
}

Mixer_Simple_Elem *
Mixer_Simple::create_mixer_elem ( snd_mixer_elem_t * elem_n )
{
  snd_mixer_selem_id_t * elem_id;
  snd_mixer_selem_id_alloca ( &elem_id );
  snd_mixer_selem_get_id ( elem_n, elem_id );

  Mixer_Simple_Elem * mx_elem (
      new Mixer_Simple_Elem ( this, snd_mixer (), elem_id ) );

  mx_elem->set_display_name ( QCoreApplication::translate (
      "ALSA::Elem_Name", mx_elem->elem_name () ) );

  // Install alsa callbacks
  snd_mixer_elem_set_callback_private ( elem_n, mx_elem );
  snd_mixer_elem_set_callback (
      elem_n, &::QSnd::Mixer_Simple_Elem::alsa_callback_mixer_elem );

  return mx_elem;
}

int
Mixer_Simple::create_socket_notifiers ()
{
  //::std::cout << "Mixer_Simple::create_socket_notifiers" << "\n";

  // Setup socket notifiers
  {
    int num_fds = snd_mixer_poll_descriptors_count ( _snd_mixer );
    if ( num_fds >= 0 ) {
      _pollfds.resize ( num_fds );
    } else {
      _err_func = "snd_mixer_poll_descriptors_count";
      _err_message = snd_error_qstring ( num_fds );
      return num_fds;
    }
  }

  if ( _pollfds.size () > 0 ) {
    memset ( _pollfds.data (), 0, _pollfds.size () * sizeof ( pollfd ) );
    int err = snd_mixer_poll_descriptors (
        _snd_mixer, _pollfds.data (), _pollfds.size () );
    if ( err < 0 ) {
      _err_func = "snd_mixer_poll_descriptors";
      _err_message = snd_error_qstring ( err );
      return err;
    }
  }

  for ( unsigned int ii = 0; ii < _pollfds.size (); ++ii ) {
    int fd ( _pollfds[ ii ].fd );
    if ( fd != 0 ) {
      QSocketNotifier * sn (
          new QSocketNotifier ( fd, QSocketNotifier::Read, this ) );

      connect ( sn,
                SIGNAL ( activated ( int ) ),
                this,
                SLOT ( socket_event ( int ) ) );

      _socket_notifiers.append ( sn );
    }
  }

  return 0;
}

void
Mixer_Simple::set_socked_notifiers_enabled ( bool flag_n )
{
  for ( int ii = 0; ii < _socket_notifiers.size (); ++ii ) {
    _socket_notifiers[ ii ]->setEnabled ( flag_n );
  }
}

void
Mixer_Simple::signalize_all_changes ()
{
  for ( int ii = 0; ii < _mixer_elems.size (); ++ii ) {
    _mixer_elems[ ii ]->signalize_changes ();
  }
}

bool
Mixer_Simple::event ( QEvent * event_n )
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
Mixer_Simple::socket_event ( int )
{
  if ( snd_mixer () != 0 ) {
    int num_ev ( snd_mixer_handle_events ( _snd_mixer ) );
    if ( num_ev < 0 ) {
      // Error. Disable socket notifiers
      set_socked_notifiers_enabled ( false );
      print_alsa_error ( "snd_mixer_handle_events ", num_ev );
    }

    signalize_all_changes ();
  }
}

void
Mixer_Simple::request_reload ()
{
  QCoreApplication::postEvent ( this,
                                new QEvent ( ::QSnd::evt_reload_request ) );
}

// Alsa callbacks

int
Mixer_Simple::alsa_callback_mixer ( snd_mixer_t * snd_mixer_n,
                                    unsigned int mask_n,
                                    snd_mixer_elem_t * )
{
  int res ( 0 );

  Mixer_Simple * qsnd_mixer;
  {
    void * priv ( snd_mixer_get_callback_private ( snd_mixer_n ) );
    qsnd_mixer = reinterpret_cast< Mixer_Simple * > ( priv );
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
        msg << "Mixer_Simple::alsa_callback_mixer: ";
        msg << "Unknown mask ( " << mask_n << " )" << ::std::endl;
        ::std::cerr << msg.str ();
      }
      res = -1;
    }
  }

  return res;
}

} // namespace QSnd
