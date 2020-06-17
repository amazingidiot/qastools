/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_pad_proxies_column_hpp__
#define __INC_wdg_pad_proxies_column_hpp__

#include "wdg/pad_proxy.hpp"
#include <QObject>

// Forward declaration
namespace Wdg
{
class Pad_Proxies_Group;
class Pad_Proxy_Slider;
class Pad_Proxy_Switch;
class Pad_Proxy_Enum;
} // namespace Wdg

namespace Wdg
{

class Pad_Proxies_Column : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Pad_Proxies_Column ( unsigned int col_idx_n = 0 );

  virtual ~Pad_Proxies_Column ();

  ::Wdg::Pad_Proxies_Group *
  group () const;

  // Column index

  unsigned int
  column_index () const;

  void
  set_column_index ( unsigned int idx_n );

  void
  clear_proxies ();

  // Slider proxy

  ::Wdg::Pad_Proxy_Slider *
  slider_proxy () const;

  void
  set_slider_proxy ( ::Wdg::Pad_Proxy_Slider * proxy_n );

  // Switch proxy

  ::Wdg::Pad_Proxy_Switch *
  switch_proxy () const;

  void
  set_switch_proxy ( ::Wdg::Pad_Proxy_Switch * proxy_n );

  // Enum proxy

  ::Wdg::Pad_Proxy_Enum *
  enum_proxy () const;

  void
  set_enum_proxy ( ::Wdg::Pad_Proxy_Enum * proxy_n );

  // Show value string

  void
  set_show_value_string ( bool flag_n );

  bool
  show_value_string () const;

  // State info

  bool
  has_slider () const;

  bool
  has_switch () const;

  bool
  has_enum () const;

  bool
  has_focus () const;

  // Value string

  virtual QString
  value_string () const;

  virtual QString
  value_min_string () const;

  virtual QString
  value_max_string () const;

  // Event handling

  bool
  event ( QEvent * event_n );

  // Signals
  signals:

  void
  sig_value_string_changed ();

  // Protected methods
  protected:
  virtual void
  slider_proxy_changed ();

  virtual void
  switch_proxy_changed ();

  virtual void
  enum_proxy_changed ();

  virtual void
  show_value_string_changed ();

  // Private attributes
  private:
  unsigned int _column_index;
  ::Wdg::Pad_Proxy_Slider * _proxy_slider;
  ::Wdg::Pad_Proxy_Switch * _proxy_switch;
  ::Wdg::Pad_Proxy_Enum * _proxy_enum;

  bool _has_focus;
  bool _show_value_string;
};

inline unsigned int
Pad_Proxies_Column::column_index () const
{
  return _column_index;
}

inline ::Wdg::Pad_Proxy_Slider *
Pad_Proxies_Column::slider_proxy () const
{
  return _proxy_slider;
}

inline ::Wdg::Pad_Proxy_Switch *
Pad_Proxies_Column::switch_proxy () const
{
  return _proxy_switch;
}

inline ::Wdg::Pad_Proxy_Enum *
Pad_Proxies_Column::enum_proxy () const
{
  return _proxy_enum;
}

inline bool
Pad_Proxies_Column::has_slider () const
{
  return ( _proxy_slider != 0 );
}

inline bool
Pad_Proxies_Column::has_switch () const
{
  return ( _proxy_switch != 0 );
}

inline bool
Pad_Proxies_Column::has_enum () const
{
  return ( _proxy_enum != 0 );
}

inline bool
Pad_Proxies_Column::show_value_string () const
{
  return _show_value_string;
}

} // namespace Wdg

#endif
