/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_tray_mixer_balloon_hpp__
#define __INC_tray_mixer_balloon_hpp__

#include "dpe/image_allocator.hpp"
#include "wdg/balloon_widget.hpp"
#include "wdg/label_width.hpp"
#include <QLabel>
#include <QPixmap>
#include <QStackedLayout>
#include <QString>

/// @brief Tray_Mixer_Balloon
///
class Tray_Mixer_Balloon : public ::Wdg::Balloon_Widget
{
  Q_OBJECT

  // Public methods
  public:
  Tray_Mixer_Balloon ( ::dpe::Image_Allocator * image_alloc_n,
                       QWidget * parent_n = 0 );

  QSize
  pixmap_size () const;

  void
  set_image_alloc ( ::dpe::Image_Allocator * image_alloc_n );

  // Volume value

  void
  set_muted ( bool flag_n );

  void
  set_permille ( unsigned int value_n );

  void
  set_icon ( const QIcon & icon_n );

  void
  set_pixmap ( const QPixmap & pixmap_n );

  bool
  eventFilter ( QObject * obj_n, QEvent * event_n );

  // Signals
  signals:

  void
  sig_activated ();

  void
  sig_middle_click ();

  void
  sig_wheel_delta ( int delta_n );

  // Protected methods
  protected:
  void
  mousePressEvent ( QMouseEvent * event_n );

  // Private attributes
  private:
  QLabel * _lbl_icon;
  QStackedLayout * _lay_volume_stack;
  ::Wdg::Label_Width * _lbl_volume_value;
  QLabel * _lbl_volume_muted;

  ::dpe::Image_Allocator * _image_alloc;

  // Strings
  QString _value_mask;
};

#endif
