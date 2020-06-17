/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_slider_hpp__
#define __INC_ds_slider_hpp__

#include "dpe/image_set_meta.hpp"
#include "wdg/cubic_curve.hpp"
#include "wdg/ds_imaging.hpp"
#include "wdg/ds_slider_meta_bg.hpp"
#include "wdg/uint_mapper.hpp"
#include <QBrush>
#include <QPen>
#include <QTime>
#include <QTimer>
#include <QWidget>

// Forward declaration
namespace dpe
{
class Image_Request;
class Image_Allocator;
} // namespace dpe
namespace Wdg
{
class DS_Widget_Style_Db;
}

namespace Wdg
{

/// @brief Dynamic sized slider
///
/// Background images:
/// 0 - non_active area
/// 1 - active area
///
/// Frame images:
/// 0 - focus
/// 1 - weak focus
///
/// Handle images:
/// 0 - idle
/// 1 - focus
/// 2 - idle + hover
/// 3 - focus + hover
/// 4 - isDown()
///
/// Marker images:
/// 0 - Current value marker
/// 1 - Value hint marker
///
class DS_Slider : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  DS_Slider ( QWidget * parent_n = 0, ::dpe::Image_Allocator * alloc_n = 0 );

  ~DS_Slider ();

  // Size hints

  QSize
  sizeHint () const;

  QSize
  minimumSizeHint () const;

  // Value index

  unsigned long
  current_index () const;

  unsigned long
  maximum_index () const;

  bool
  slider_down () const;

  // Image allocator and style id

  ::dpe::Image_Allocator *
  image_alloc () const;

  void
  set_image_alloc ( ::dpe::Image_Allocator * alloc_n );

  // Style database

  const ::Wdg::DS_Widget_Style_Db *
  wdg_style_db () const;

  void
  set_wdg_style_db ( const ::Wdg::DS_Widget_Style_Db * db_n );

  // Style id

  unsigned int
  style_id () const;

  void
  set_style_id ( unsigned int id_n );

  // Additional config

  ::Wdg::DS_Slider_Meta_Bg &
  meta_bg ();

  // Mouse wheel degrees

  void
  set_wheel_degrees ( unsigned int degrees_n );

  unsigned int
  wheel_degrees () const;

  // Public slots
  public slots:

  void
  set_current_index ( unsigned long idx_n );

  void
  adjust_current_index ( long idx_delta_n );

  void
  set_maximum_index ( unsigned long idx_n );

  // Signals
  signals:

  void
  sig_current_index_changed ( unsigned long idx_n );

  void
  sig_maximum_index_changed ( unsigned long idx_n );

  // Protected methods
  protected:
  void
  set_slider_down ( bool down_n );

  virtual void
  current_index_changed ();

  virtual void
  maximum_index_changed ();

  // Mapping: Pixel <=> Value

  unsigned long
  map_handle_pos_to_index ( int handle_pos_n );

  unsigned int
  map_index_to_handle_pos ( unsigned long idx_n );

  unsigned int
  map_tick_pos_to_handle_pos ( int tick_pos_n );

  unsigned int
  map_handle_pos_to_tick_pos ( int handle_pos_n );

  unsigned long
  map_tick_pos_to_index ( int tick_pos_n );

  unsigned int
  map_index_to_tick_pos ( unsigned long idx_n );

  void
  clear_index_mappers ();

  void
  update_index_mappers ();

  // Cursor mode

  bool
  update_cursor_mode ();

  // Handle position

  /// @return true if the handle position was changed
  bool
  set_handle_pos ( unsigned int pos_n );

  void
  update_value_from_handle_pos ();

  void
  finish_handle_manipulation ();

  // Change events

  void
  changeEvent ( QEvent * event_n );

  // Focus events

  void
  focusInEvent ( QFocusEvent * event_n );

  void
  focusOutEvent ( QFocusEvent * event_n );

  // Enter events

  void
  enterEvent ( QEvent * event_n );

  void
  leaveEvent ( QEvent * event_n );

  // Mouse events

  void
  mousePressEvent ( QMouseEvent * event_n );

  void
  mouseReleaseEvent ( QMouseEvent * event_n );

  void
  mouseMoveEvent ( QMouseEvent * event_n );

  void
  wheelEvent ( QWheelEvent * event_n );

  // Keyboard events

  void
  keyPressEvent ( QKeyEvent * event_n );

  void
  keyReleaseEvent ( QKeyEvent * event_n );

  // Window events

  void
  resizeEvent ( QResizeEvent * event_n );

  void
  paintEvent ( QPaintEvent * event_n );

  /// @brief Fetches new pixmaps from the image buffers
  void
  update_pixmaps ();

  void
  update_painter_utils ();

  // Private slots
  private slots:

  void
  anim_tick ();

  // Private methods
  private:
  void
  anim_stop ();

  void
  anim_snap_start ();

  bool
  anim_snap_tick ( unsigned int msec_n );

  // Private attributes
  private:
  static const unsigned int num_images_bg = 2;
  static const unsigned int num_images_marker = 2;
  static const unsigned int num_images_frame = 2;
  static const unsigned int num_images_handle = 5;

  ::Wdg::DS_Imaging _dsi;

  unsigned long _current_index;
  unsigned long _maximum_index;

  unsigned int _wheel_degrees;

  long _step_single;
  long _step_page;

  QRect _handle_rect;
  QPen _snap_pen;
  QBrush _snap_brush;

  unsigned int _handle_pos;
  unsigned int _handle_pos_index;
  unsigned int _handle_pos_hint;
  unsigned int _handle_pos_max;
  int _marker_offset[ 2 ];
  int _tick_min;
  int _tick_max;
  QPoint _mouse_last;

  UInt_Mapper * _map_hp_idx;
  UInt_Mapper * _map_idx_hp;

  bool _slider_down;
  bool _update_pixmaps_pending;
  bool _update_painter_pending;
  bool _cursor_select_allowed;
  bool _cursor_over_handle;
  bool _cursor_value_hinting;
  bool _anim_run_snap;

  // Animation
  QTimer _anim_timer;

  QTime _anim_snap_time;
  unsigned int _anim_snap_msec_max;
  ::Wdg::Cubic_Curve _cubic_curve;

  // Graphics creation
  QSize _marker_size;

  // Pixmap buffers
  ::Wdg::DS_Slider_Meta_Bg _meta_bg;
  ::dpe::Image_Set_Meta _meta_marker;
  ::dpe::Image_Set_Meta _meta_frame;
  ::dpe::Image_Set_Meta _meta_handle;
  const ::Wdg::DS_Widget_Style_Db * _wdg_style_db;
};

inline unsigned long
DS_Slider::current_index () const
{
  return _current_index;
}

inline unsigned long
DS_Slider::maximum_index () const
{
  return _maximum_index;
}

inline bool
DS_Slider::slider_down () const
{
  return _slider_down;
}

inline unsigned int
DS_Slider::wheel_degrees () const
{
  return _wheel_degrees;
}

inline ::dpe::Image_Allocator *
DS_Slider::image_alloc () const
{
  return _dsi.image_alloc ();
}

inline const ::Wdg::DS_Widget_Style_Db *
DS_Slider::wdg_style_db () const
{
  return _wdg_style_db;
}

inline unsigned int
DS_Slider::style_id () const
{
  return _meta_bg.style_id;
}

inline ::Wdg::DS_Slider_Meta_Bg &
DS_Slider::meta_bg ()
{
  return _meta_bg;
}

} // namespace Wdg

#endif
