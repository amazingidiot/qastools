/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_slider_status_widget_hpp__
#define __INC_slider_status_widget_hpp__

#include "wdg/label_elide.hpp"
#include "wdg/label_width.hpp"
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLocale>
#include <QSpinBox>
#include <QWidget>

// Forward declaration
namespace Wdg
{
class Sliders_Pad;
}

namespace MWdg
{

/// @brief Slider_Status_Widget
///
class Slider_Status_Widget : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Slider_Status_Widget ( QWidget * parent_n = 0 );

  ~Slider_Status_Widget ();

  // Sliders pad

  ::Wdg::Sliders_Pad *
  sliders_pad () const;

  void
  set_sliders_pad ( ::Wdg::Sliders_Pad * pad_n );

  // Value string generators

  QString
  volume_string ( long volume_n ) const;

  QString
  db_string ( double db_val_n ) const;

  // Element info

  virtual QString
  elem_name () const = 0;

  virtual bool
  elem_has_volume () const = 0;

  virtual bool
  elem_has_dB () const = 0;

  virtual long
  elem_volume_value () const = 0;

  virtual void
  elem_set_volume ( long value_n ) const = 0;

  virtual long
  elem_volume_min () const = 0;

  virtual long
  elem_volume_max () const = 0;

  virtual long
  elem_dB_value () const = 0;

  virtual void
  elem_set_nearest_dB ( long dB_value_n ) const = 0;

  virtual long
  elem_dB_min () const = 0;

  virtual long
  elem_dB_max () const = 0;

  // Slider selection

  virtual void
  select_slider ( unsigned int grp_idx_n, unsigned int col_idx_n ) = 0;

  // Public slots
  public slots:

  void
  mixer_values_changed ();

  void
  slider_focus_changed ();

  // Protected slots
  protected slots:

  void
  update_values ();

  void
  volume_value_changed ( int value_n );

  void
  db_value_changed ( double value_n );

  // Protected methods
  protected:
  void
  setup_values ();

  void
  contextMenuEvent ( QContextMenuEvent * event_n );

  // Private attributes
  private:
  ::Wdg::Sliders_Pad * _sliders_pad;

  // Widgets
  ::Wdg::Label_Elide _elem_name;

  QLabel _lbl_value;
  QLabel _lbl_max;
  QLabel _lbl_min;

  QLabel _volume_title;
  QSpinBox _volume_value;
  ::Wdg::Label_Width _volume_max;
  ::Wdg::Label_Width _volume_min;

  QLabel _db_title;
  QDoubleSpinBox _db_value;
  ::Wdg::Label_Width _db_max;
  ::Wdg::Label_Width _db_min;

  QLocale _loc;
};

inline Wdg::Sliders_Pad *
Slider_Status_Widget::sliders_pad () const
{
  return _sliders_pad;
}

inline QString
Slider_Status_Widget::volume_string ( long volume_n ) const
{
  return _loc.toString ( (int)volume_n );
}

inline QString
Slider_Status_Widget::db_string ( double db_val_n ) const
{
  return _loc.toString ( db_val_n, 'f', 2 );
}

} // namespace MWdg

#endif
