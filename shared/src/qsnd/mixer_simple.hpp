/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_simple_hpp__
#define __INC_qsnd_mixer_simple_hpp__

#include "qsnd/alsa.hpp"
#include <QList>
#include <QObject>
#include <QSocketNotifier>
#include <vector>

// Forward declaration
namespace QSnd
{
class Mixer_Simple_Elem;
class Mixer_Simple_Filter;
} // namespace QSnd

namespace QSnd
{

/// @brief Mixer_Simple
///
/// Connects ALSA simple mixer objects with Qt objects
///
class Mixer_Simple : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Simple ( QObject * parent_n = 0 );

  ~Mixer_Simple ();

  snd_hctl_t *
  snd_hctl ();

  snd_mixer_t *
  snd_mixer ();

  // Elements

  unsigned int
  num_elems () const;

  /// @brief Number of elements in playback (0) or capture (1)
  unsigned int
  num_elems_stream ( unsigned int stream_dir_n ) const;

  /// @brief Elements with a volume
  unsigned int
  num_elems_volume ( unsigned int stream_dir_n ) const;

  /// @brief Elements with no volume
  unsigned int
  num_elems_no_volume ( unsigned int stream_dir_n ) const;

  const Mixer_Simple_Elem *
  elem ( unsigned int idx_n ) const;

  Mixer_Simple_Elem *
  elem ( unsigned int idx_n );

  // Element filters

  void
  clear_elem_filters ();

  /// @brief Appends a new element filter
  ///
  /// The ownership of the filter is passed to the mixer.
  void
  append_elem_filter ( ::QSnd::Mixer_Simple_Filter * filter_n );

  // open / close

  /// @brief Opens a simple mixer
  ///
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

  // Event handling

  bool
  event ( QEvent * event_n );

  // Alsa callbacks

  static int
  alsa_callback_mixer ( snd_mixer_t * snd_mixer_n,
                        unsigned int mask_n,
                        snd_mixer_elem_t * );

  // Signals
  signals:

  void
  sig_mixer_reload_request ();

  // Public slots
  public slots:

  void
  request_reload ();

  // Private slots
  private slots:

  void
  socket_event ( int socket_id );

  // Private methods
  private:
  // Mixer elements

  void
  clear_mixer_elems ();

  int
  create_mixer_elems ();

  void
  destroy_mixer_elem ( Mixer_Simple_Elem * elem_n );

  Mixer_Simple_Elem *
  create_mixer_elem ( snd_mixer_elem_t * elem_n );

  // Socket notifiers

  int
  create_socket_notifiers ();

  void
  set_socked_notifiers_enabled ( bool flag_n );

  void
  signalize_all_changes ();

  // Private attributes
  private:
  snd_hctl_t * _snd_hctl;
  snd_mixer_t * _snd_mixer;

  QList<::QSnd::Mixer_Simple_Elem * > _mixer_elems;
  ::std::vector< pollfd > _pollfds;
  QList< QSocketNotifier * > _socket_notifiers;

  // Statistics
  unsigned int _num_elems_stream[ 2 ];
  unsigned int _num_elems_volume[ 2 ];    // Elements with a volume
  unsigned int _num_elems_no_volume[ 2 ]; // Elements without a volume

  bool _update_requested;
  bool _reload_requested;

  QString _err_func;
  QString _err_message;

  QList<::QSnd::Mixer_Simple_Filter * > _elem_filters;
};

inline bool
Mixer_Simple::is_open () const
{
  return ( _snd_mixer != 0 );
}

inline const QString &
Mixer_Simple::err_func () const
{
  return _err_func;
}

inline const QString &
Mixer_Simple::err_message () const
{
  return _err_message;
}

inline snd_hctl_t *
Mixer_Simple::snd_hctl ()
{
  return _snd_hctl;
}

inline snd_mixer_t *
Mixer_Simple::snd_mixer ()
{
  return _snd_mixer;
}

// Elements

inline unsigned int
Mixer_Simple::num_elems () const
{
  return _mixer_elems.size ();
}

inline unsigned int
Mixer_Simple::num_elems_stream ( unsigned int stream_dir_n ) const
{
  return _num_elems_stream[ stream_dir_n ];
}

inline unsigned int
Mixer_Simple::num_elems_volume ( unsigned int stream_dir_n ) const
{
  return _num_elems_volume[ stream_dir_n ];
}

inline unsigned int
Mixer_Simple::num_elems_no_volume ( unsigned int stream_dir_n ) const
{
  return _num_elems_no_volume[ stream_dir_n ];
}

inline const Mixer_Simple_Elem *
Mixer_Simple::elem ( unsigned int idx_n ) const
{
  return _mixer_elems[ idx_n ];
}

inline Mixer_Simple_Elem *
Mixer_Simple::elem ( unsigned int idx_n )
{
  return _mixer_elems[ idx_n ];
}

} // namespace QSnd

#endif
