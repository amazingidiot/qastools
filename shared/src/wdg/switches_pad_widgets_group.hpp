/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_switches_pad_widgets_group_hpp__
#define __INC_switches_pad_widgets_group_hpp__

#include "wdg/pad_proxies_group.hpp"
#include "wdg/switches_pad_widgets.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPen>
#include <QVBoxLayout>
#include <QWidget>

namespace Wdg
{

/// @brief Switches_Pad_Widgets_Group
///
class Switches_Pad_Widgets_Group : public QWidget
{
  // Public typedefs
  public:
  typedef QList<::Wdg::Switches_Pad_Widgets * > Widgets_List;

  // Public methods
  public:
  Switches_Pad_Widgets_Group ( QWidget * parent_n = 0 );

  ~Switches_Pad_Widgets_Group ();

  void
  clear_widgets ();

  // Label

  QLabel *
  label ();

  void
  set_label ( QLabel * wdg_n );

  // Widgets list

  unsigned int
  num_widgets () const;

  void
  append_widgets ( ::Wdg::Switches_Pad_Widgets * wdgs_n );

  const Switches_Pad_Widgets *
  widgets ( int idx_n ) const;

  ::Wdg::Switches_Pad_Widgets *
  widgets ( int idx_n );

  // Stem pen

  void
  set_stem_pen ( const QPen & pen_n );

  // Protected methods
  private:
  QVBoxLayout *
  lay_vbox ();

  int
  enum_spacing () const;

  // Event handling

  bool
  eventFilter ( QObject * watched_n, QEvent * event_n );

  void
  paintEvent ( QPaintEvent * event_n );

  // Private attributes
  private:
  Widgets_List _widgets;
  QLabel * _label;
  QGridLayout * _lay_grid;
  QPen _stem_pen;
};

inline QLabel *
Switches_Pad_Widgets_Group::label ()
{
  return _label;
}

inline unsigned int
Switches_Pad_Widgets_Group::num_widgets () const
{
  return _widgets.size ();
}

inline const ::Wdg::Switches_Pad_Widgets *
Switches_Pad_Widgets_Group::widgets ( int idx_n ) const
{
  return _widgets[ idx_n ];
}

inline ::Wdg::Switches_Pad_Widgets *
Switches_Pad_Widgets_Group::widgets ( int idx_n )
{
  return _widgets[ idx_n ];
}

inline QVBoxLayout *
Switches_Pad_Widgets_Group::lay_vbox ()
{
  return static_cast< QVBoxLayout * > ( layout () );
}

inline void
Switches_Pad_Widgets_Group::set_stem_pen ( const QPen & pen_n )
{
  _stem_pen = pen_n;
}

typedef QList<::Wdg::Switches_Pad_Widgets_Group * >
    Switches_Pad_Widgets_Group_List;

} // namespace Wdg

#endif
