/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_hctl_hpp__
#define __INC_qsnd_mixer_hctl_hpp__

#include "qsnd/alsa.hpp"
#include "qsnd/event_types.hpp"
#include <QList>
#include <QObject>
#include <QSocketNotifier>
#include <vector>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Info_Db;
class Mixer_HCTL_Elem;
class Mixer_HCTL_Elem_Group;
} // namespace QSnd

namespace QSnd
{

/// @brief Mixer_HCTL
///
/// Brings Qt and ALSA objects together but without
/// any GUI objects
///
class Mixer_HCTL : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL ( QObject * parent_n = 0 );

  ~Mixer_HCTL ();

  const Mixer_HCTL_Info_Db *
  info_db () const;

  snd_hctl_t *
  snd_hctl ();

  // Init / close

  /// @return On an error a negative error code
  int
  open ( const QString & dev_name_n );

  void
  close ();

  bool
  is_open () const;

  // Error strings / codes

  const QString &
  err_func () const;

  const QString &
  err_message () const;

  // Elements

  unsigned int
  num_elems () const;

  const Mixer_HCTL_Elem *
  elem ( unsigned int idx_n ) const;

  Mixer_HCTL_Elem *
  elem ( unsigned int idx_n );

  // Interface types

  unsigned int
  iface_type_count ( unsigned int type_idx_n ) const;

  unsigned int
  iface_types_avail () const;

  unsigned int
  iface_avail_type ( unsigned int type_idx_n ) const;

  // Alsa callbacks

  static int
  alsa_callback_hctl ( snd_hctl_t * snd_hctl_n,
                       unsigned int mask_n,
                       snd_hctl_elem_t * elem_n );

  // Signals
  signals:

  void
  sig_mixer_reload_request ();

  // Public slots
  public slots:

  void
  request_reload ();

  // Protected slots
  protected slots:

  void
  socket_event ( int socket_id );

  // Protected methods
  protected:
  int
  create_mixer_elems ();

  // Socket notifiers

  int
  create_socket_notifiers ();

  void
  set_socked_notifiers_enabled ( bool flag_n );

  void
  signalize_all_changes ();

  bool
  event ( QEvent * event_n );

  // Private attributes
  private:
  ::QSnd::Mixer_HCTL_Info_Db * _info_db;
  snd_hctl_t * _snd_hctl;

  QList< Mixer_HCTL_Elem * > _mixer_elems;
  std::vector< pollfd > _pollfds;
  QList< QSocketNotifier * > _socket_notifiers;

  unsigned int _iface_num_types;
  unsigned int _iface_avail_types[ 7 ];
  unsigned int _iface_type_count[ 7 ];

  bool _update_requested;
  bool _reload_requested;

  QString _err_func;
  QString _err_message;
};

inline const Mixer_HCTL_Info_Db *
Mixer_HCTL::info_db () const
{
  return _info_db;
}

inline snd_hctl_t *
Mixer_HCTL::snd_hctl ()
{
  return _snd_hctl;
}

inline bool
Mixer_HCTL::is_open () const
{
  return ( _snd_hctl != 0 );
}

inline const QString &
Mixer_HCTL::err_func () const
{
  return _err_func;
}

inline const QString &
Mixer_HCTL::err_message () const
{
  return _err_message;
}

// Elements

inline unsigned int
Mixer_HCTL::num_elems () const
{
  return _mixer_elems.size ();
}

inline const Mixer_HCTL_Elem *
Mixer_HCTL::elem ( unsigned int idx_n ) const
{
  return _mixer_elems[ idx_n ];
}

inline Mixer_HCTL_Elem *
Mixer_HCTL::elem ( unsigned int idx_n )
{
  return _mixer_elems[ idx_n ];
}

// Interface types

inline unsigned int
Mixer_HCTL::iface_type_count ( unsigned int type_idx_n ) const
{
  return _iface_type_count[ type_idx_n ];
}

inline unsigned int
Mixer_HCTL::iface_types_avail () const
{
  return _iface_num_types;
}

inline unsigned int
Mixer_HCTL::iface_avail_type ( unsigned int type_idx_n ) const
{
  return _iface_avail_types[ type_idx_n ];
}

} // namespace QSnd

#endif
