/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_switches_pad_hpp__
#define __INC_wdg_switches_pad_hpp__

#include "wdg/pad_focus_info.hpp"
#include <QList>
#include <QPen>
#include <QWidget>

// Forward declaration
namespace Wdg
{
class Pad_Proxies_Group;
class Switches_Pad_Widgets_Group;
} // namespace Wdg

namespace Wdg
{

/// @brief Switches_Pad
///
class Switches_Pad : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Switches_Pad ( QWidget * parent_n = 0 );

  ~Switches_Pad ();

  void
  set_viewport_geometry ( const QRect & rect_n );

  // Proxies groups

  const QList<::Wdg::Pad_Proxies_Group * > &
  proxies_groups () const;

  void
  set_proxies_groups ( const QList<::Wdg::Pad_Proxies_Group * > & groups_n );

  void
  clear_proxies_groups ();

  // Proxies groups access

  unsigned int
  num_proxies_groups () const;

  ::Wdg::Pad_Proxies_Group *
  proxies_group ( unsigned int idx_n );

  // Focus item getting / setting

  const ::Wdg::Pad_Focus_Info &
  focus_info ();

  // Event handling

  bool
  event ( QEvent * event_n );

  // Public slots
  public slots:

  void
  set_focus_proxy ( unsigned int proxies_group_idx_n );

  void
  set_focus_proxy ( unsigned int proxies_group_idx_n,
                    unsigned int proxy_idx_n );

  // Public signals
  signals:

  void
  sig_focus_changed ();

  // Protected methods
  protected:
  void
  clear_widgets_groups ();

  void
  create_widgets_groups ();

  void
  update_colors ();

  // Private attributes
  private:
  QList<::Wdg::Pad_Proxies_Group * > _proxies_groups;
  QList<::Wdg::Switches_Pad_Widgets_Group * > _widgets_groups;

  QRect _viewport;
  Pad_Focus_Info _focus_info;

  QPen _stem_pen;
};

inline const QList<::Wdg::Pad_Proxies_Group * > &
Switches_Pad::proxies_groups () const
{
  return _proxies_groups;
}

inline ::Wdg::Pad_Proxies_Group *
Switches_Pad::proxies_group ( unsigned int idx_n )
{
  return _proxies_groups[ idx_n ];
}

inline unsigned int
Switches_Pad::num_proxies_groups () const
{
  return _proxies_groups.size ();
}

inline const Pad_Focus_Info &
Switches_Pad::focus_info ()
{
  return _focus_info;
}

} // namespace Wdg

#endif
