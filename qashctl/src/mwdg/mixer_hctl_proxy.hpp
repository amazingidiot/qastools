/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_proxy_hpp__
#define __INC_mixer_hctl_proxy_hpp__

#include <QObject>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Elem;
}

namespace MWdg
{

/// @brief Mixer_HCTL_Proxy
///
class Mixer_HCTL_Proxy : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Proxy ( QObject * parent_n = 0 );

  virtual ~Mixer_HCTL_Proxy ();

  // Snd element

  ::QSnd::Mixer_HCTL_Elem *
  snd_elem () const;

  void
  set_snd_elem ( ::QSnd::Mixer_HCTL_Elem * elem_n );

  // Snd element index

  unsigned int
  elem_idx () const;

  void
  set_elem_idx ( unsigned int idx_n );

  // Enabled

  bool
  is_enabled () const;

  void
  set_enabled ( bool flag_n );

  // Joined

  bool
  is_joined () const;

  void
  set_joined ( bool flag_n );

  bool
  joined_by_key () const;

  // Focus

  bool
  has_focus () const;

  bool
  eventFilter ( QObject * obj_n, QEvent * event_n );

  // Signals
  signals:

  void
  sig_enabled_changed ( bool flag_n );

  // Public slots
  public:
  virtual void
  update_value_from_source ();

  // Private attributes
  private:
  ::QSnd::Mixer_HCTL_Elem * _snd_elem;
  unsigned int _elem_idx;

  bool _is_enabled;
  bool _is_joined;
  bool _has_focus;
};

inline ::QSnd::Mixer_HCTL_Elem *
Mixer_HCTL_Proxy::snd_elem () const
{
  return _snd_elem;
}

inline unsigned int
Mixer_HCTL_Proxy::elem_idx () const
{
  return _elem_idx;
}

inline bool
Mixer_HCTL_Proxy::is_joined () const
{
  return _is_joined;
}

inline bool
Mixer_HCTL_Proxy::is_enabled () const
{
  return _is_enabled;
}

inline bool
Mixer_HCTL_Proxy::has_focus () const
{
  return _has_focus;
}

} // namespace MWdg

#endif
