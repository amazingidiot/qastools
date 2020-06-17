/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_slider.hpp"
#include "dpe/image_allocator.hpp"
#include "dpe/image_request.hpp"
#include "dpe/image_set.hpp"
#include "wdg/color_methods.hpp"
#include "wdg/ds_widget_style_db.hpp"
#include "wdg/ds_widget_types.hpp"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>

namespace Wdg
{

DS_Slider::DS_Slider ( QWidget * parent_n, ::dpe::Image_Allocator * alloc_n )
: QWidget ( parent_n )
, _dsi ( 4, alloc_n )
, _current_index ( 0 )
, _maximum_index ( 0 )
, _wheel_degrees ( 720 )
, _step_single ( 0 )
, _step_page ( 0 )
, _handle_pos ( 0 )
, _handle_pos_index ( 0 )
, _handle_pos_hint ( 0 )
, _handle_pos_max ( 0 )
, _map_hp_idx ( 0 )
, _map_idx_hp ( 0 )
, _slider_down ( false )
, _update_pixmaps_pending ( false )
, _update_painter_pending ( false )
, _cursor_select_allowed ( false )
, _cursor_over_handle ( false )
, _cursor_value_hinting ( false )
, _anim_run_snap ( false )
, _meta_bg ( num_images_bg, ::Wdg::DS_SLIDER, 0 )
, _meta_marker ( num_images_marker, ::Wdg::DS_SLIDER, 1 )
, _meta_frame ( num_images_frame, ::Wdg::DS_SLIDER, 2 )
, _meta_handle ( num_images_handle, ::Wdg::DS_SLIDER, 3 )
, _wdg_style_db ( 0 )
{
  _dsi.image_request ()->meta[ 0 ] = &_meta_bg;
  _dsi.image_request ()->meta[ 1 ] = &_meta_marker;
  _dsi.image_request ()->meta[ 2 ] = &_meta_frame;
  _dsi.image_request ()->meta[ 3 ] = &_meta_handle;

  {
    QSizePolicy policy ( sizePolicy () );
    policy.setHorizontalPolicy ( QSizePolicy::Expanding );
    policy.setVerticalPolicy ( QSizePolicy::Expanding );
    setSizePolicy ( policy );
  }
  setFocusPolicy ( Qt::StrongFocus );
  setMouseTracking ( true );

  // Animation timer
  {
    const unsigned int fps ( 60 );
    _anim_timer.setInterval ( 1000 / fps );
    connect (
        &_anim_timer, SIGNAL ( timeout () ), this, SLOT ( anim_tick () ) );
  }

  update_index_mappers ();
  _update_painter_pending = true;
}

DS_Slider::~DS_Slider ()
{
  _dsi.set_image_alloc ( 0 );
  clear_index_mappers ();
}

QSize
DS_Slider::sizeHint () const
{
  const int def_len ( fontMetrics ().height () );
  return QSize ( def_len, def_len * 10 );
}

QSize
DS_Slider::minimumSizeHint () const
{
  const int def_len ( fontMetrics ().height () );
  return QSize ( def_len, def_len * 4 );
}

void
DS_Slider::set_image_alloc ( ::dpe::Image_Allocator * alloc_n )
{
  _dsi.set_image_alloc ( alloc_n );
}

void
DS_Slider::set_wdg_style_db ( const ::Wdg::DS_Widget_Style_Db * db_n )
{
  if ( _wdg_style_db != db_n ) {
    _wdg_style_db = db_n;
    _update_painter_pending = true;
  }
}

void
DS_Slider::set_style_id ( unsigned int id_n )
{
  if ( style_id () != id_n ) {
    _dsi.set_images_style_id ( id_n );
    _update_painter_pending = true;
  }
}

void
DS_Slider::set_wheel_degrees ( unsigned int degrees_n )
{
  _wheel_degrees = ( degrees_n > 0 ) ? degrees_n : 1;
}

void
DS_Slider::set_current_index ( unsigned long idx_n )
{
  if ( idx_n > maximum_index () ) {
    idx_n = maximum_index ();
  }
  if ( current_index () != idx_n ) {
    _current_index = idx_n;
    this->current_index_changed ();
    emit sig_current_index_changed ( idx_n );
  }
}

void
DS_Slider::adjust_current_index ( long idx_delta_n )
{
  unsigned long idx ( current_index () );
  if ( idx_delta_n >= 0 ) {
    unsigned long dplus ( idx_delta_n );
    if ( dplus >= maximum_index () ) {
      idx = maximum_index ();
    } else {
      if ( ( maximum_index () - dplus ) <= idx ) {
        idx = maximum_index ();
      } else {
        idx += dplus;
      }
    }
  } else {
    unsigned long dplus ( -idx_delta_n );
    if ( dplus > idx ) {
      idx = 0;
    } else {
      idx -= dplus;
    }
  }
  set_current_index ( idx );
}

void
DS_Slider::set_maximum_index ( unsigned long idx_n )
{
  if ( maximum_index () != idx_n ) {
    _maximum_index = idx_n;
    this->maximum_index_changed ();
    emit sig_maximum_index_changed ( idx_n );
  }
}

void
DS_Slider::set_slider_down ( bool down_n )
{
  _slider_down = down_n;
}

void
DS_Slider::current_index_changed ()
{
  _handle_pos_index = map_index_to_handle_pos ( current_index () );
  if ( !slider_down () ) {
    anim_snap_start ();
  }
  update ();
}

void
DS_Slider::maximum_index_changed ()
{
  update_index_mappers ();

  if ( current_index () > maximum_index () ) {
    set_current_index ( maximum_index () );
  }

  unsigned long delta ( maximum_index () );
  unsigned long div;
  unsigned long val;
  unsigned long mod;

  // Single step
  div = 100;
  val = delta / div;
  mod = delta % div;
  if ( mod > ( div / 2 ) ) {
    ++val;
  }
  if ( val == 0 ) {
    ++val;
  }
  _step_single = val;

  // Page step
  div = 12;
  val = delta / div;
  mod = delta % div;
  if ( mod > ( div / 2 ) ) {
    ++val;
  }
  if ( val == 0 ) {
    ++val;
  }
  _step_page = val;

  // Update pixmap config
  _meta_bg.ticks_max_idx = delta;

  update_pixmaps ();
}

unsigned long
DS_Slider::map_handle_pos_to_index ( int handle_pos_n )
{
  unsigned long res ( 0 );

  if ( handle_pos_n <= 0 ) {
    res = maximum_index ();
  } else {
    unsigned int hpos ( handle_pos_n );
    if ( hpos <= _handle_pos_max ) {
      hpos = _handle_pos_max - hpos;
      res = _map_hp_idx->map ( hpos );
    }
  }

  return res;
}

unsigned int
DS_Slider::map_index_to_handle_pos ( unsigned long idx_n )
{
  unsigned int res ( _handle_pos_max );

  if ( idx_n <= maximum_index () ) {
    res = _map_idx_hp->map ( idx_n );
    if ( res > _handle_pos_max ) {
      res = 0;
    } else {
      res = _handle_pos_max - res;
    }
  }

  return res;
}

unsigned int
DS_Slider::map_tick_pos_to_handle_pos ( int tick_pos_n )
{
  unsigned int res ( 0 );
  const int delta ( _handle_rect.height () / 2 );
  if ( tick_pos_n > delta ) {
    res = ( tick_pos_n - delta );
    if ( res > _handle_pos_max ) {
      res = _handle_pos_max;
    }
  }
  return res;
}

unsigned int
DS_Slider::map_handle_pos_to_tick_pos ( int handle_pos_n )
{
  unsigned int res ( 0 );
  if ( handle_pos_n > 0 ) {
    res = handle_pos_n;
    if ( res > _handle_pos_max ) {
      res = _handle_pos_max;
    }
  }
  res += ( _handle_rect.height () / 2 );
  return res;
}

unsigned int
DS_Slider::map_index_to_tick_pos ( unsigned long idx_n )
{
  unsigned int res = map_index_to_handle_pos ( idx_n );
  res = map_handle_pos_to_tick_pos ( res );
  return res;
}

unsigned long
DS_Slider::map_tick_pos_to_index ( int tick_pos_n )
{
  unsigned long res = map_tick_pos_to_handle_pos ( tick_pos_n );
  res = map_handle_pos_to_index ( res );
  return res;
}

void
DS_Slider::clear_index_mappers ()
{
  if ( _map_idx_hp != 0 ) {
    delete _map_idx_hp;
    _map_idx_hp = 0;
  }
  if ( _map_hp_idx != 0 ) {
    delete _map_hp_idx;
    _map_hp_idx = 0;
  }
}

void
DS_Slider::update_index_mappers ()
{
  clear_index_mappers ();

  if ( maximum_index () > _handle_pos_max ) {
    _map_idx_hp = new UInt_Mapper_Down ( maximum_index (), _handle_pos_max );
    _map_hp_idx = new UInt_Mapper_Up ( _handle_pos_max, maximum_index () );
  } else {
    _map_idx_hp = new UInt_Mapper_Up ( maximum_index (), _handle_pos_max );
    _map_hp_idx = new UInt_Mapper_Down ( _handle_pos_max, maximum_index () );
  }
}

bool
DS_Slider::update_cursor_mode ()
{
  //::std::cout << "DS_Slider::update_cursor_mode " << "\n";

  bool res ( false ); // Indicates a cursor mode change

  const QPoint cur_pos ( mapFromGlobal ( QCursor::pos () ) );

  { // Check if the cursor is over the handle
    const bool over_handle ( _handle_rect.contains ( cur_pos ) );
    if ( over_handle != _cursor_over_handle ) {
      _cursor_over_handle = over_handle;
      res = true;
    }
  }

  { // Update cursor value hinting
    const bool show_hint ( _cursor_select_allowed &&
                           rect ().contains ( cur_pos ) &&
                           !_cursor_over_handle && !slider_down () );

    if ( show_hint ) {
      unsigned long hpos;
      hpos = map_tick_pos_to_index ( cur_pos.y () );
      hpos = map_index_to_handle_pos ( hpos );

      if ( _handle_pos_hint != hpos ) {
        _handle_pos_hint = hpos;
        res = true;
      }
    }

    if ( _cursor_value_hinting != show_hint ) {
      _cursor_value_hinting = show_hint;
      res = true;
    }
  }

  { // Update cursor shape
    Qt::CursorShape cshape ( Qt::ArrowCursor );
    if ( slider_down () ) {
      cshape = Qt::ClosedHandCursor;
    } else if ( _cursor_over_handle ) {
      cshape = Qt::OpenHandCursor;
    } else if ( _cursor_value_hinting ) {
      cshape = Qt::CrossCursor;
    }

    QCursor curs ( cursor () );
    if ( curs.shape () != cshape ) {
      curs.setShape ( cshape );
      setCursor ( curs );
      res = true;
    }
  }

  return res;
}

bool
DS_Slider::set_handle_pos ( unsigned int pos_n )
{
  //::std::cout << "DS_Slider::set_handle_pos " << pos_n << "\n";

  if ( pos_n > _handle_pos_max ) {
    pos_n = _handle_pos_max;
  }

  if ( _handle_pos != pos_n ) {
    _handle_pos = pos_n;
    _handle_rect.moveTop ( _handle_pos );
    return true;
  }

  return false;
}

void
DS_Slider::update_value_from_handle_pos ()
{
  unsigned long idx = map_handle_pos_to_index ( _handle_pos );
  set_current_index ( idx );
}

void
DS_Slider::finish_handle_manipulation ()
{
  set_slider_down ( false );

  _handle_pos_index = map_index_to_handle_pos ( current_index () );
  anim_snap_start ();

  update_cursor_mode ();
}

void
DS_Slider::changeEvent ( QEvent * event_n )
{
  QWidget::changeEvent ( event_n );

  bool upd_painter ( false );
  bool upd_pixmaps ( false );

  switch ( event_n->type () ) {
  case QEvent::ActivationChange:
  case QEvent::EnabledChange:
    upd_pixmaps = true;
    upd_painter = true;
    break;
  case QEvent::StyleChange:
  case QEvent::PaletteChange:
  case QEvent::LayoutDirectionChange:
    upd_painter = true;
    break;

  default:
    break;
  }

  if ( upd_painter ) {
    _update_painter_pending = true;
  }
  if ( upd_pixmaps ) {
    update_pixmaps ();
  }
}

// Focus events
void
DS_Slider::focusInEvent ( QFocusEvent * event_n )
{
  //::std::cout << "DS_Slider::focusInEvent " << "\n";

  QWidget::focusInEvent ( event_n );

  switch ( event_n->reason () ) {
  case Qt::MouseFocusReason:
  case Qt::ActiveWindowFocusReason:
    _cursor_select_allowed = false;
    break;

  default:
    _cursor_select_allowed = true;
    break;
  }

  update_cursor_mode ();
  update ();
}

void
DS_Slider::focusOutEvent ( QFocusEvent * event_n )
{
  //::std::cout << "DS_Slider::focusOutEvent " << "\n";

  QWidget::focusOutEvent ( event_n );

  _cursor_select_allowed = false;
  _cursor_value_hinting = false;
  finish_handle_manipulation ();
  update ();
}

void
DS_Slider::enterEvent ( QEvent * event_n )
{
  //::std::cout << "DS_Slider::enterEvent" << "\n";

  QWidget::enterEvent ( event_n );

  update_cursor_mode ();
  update ();
}

void
DS_Slider::leaveEvent ( QEvent * event_n )
{
  //::std::cout << "DS_Slider::leaveEvent" << "\n";

  QWidget::leaveEvent ( event_n );

  update_cursor_mode ();
  update ();
}

// Mouse events
void
DS_Slider::mousePressEvent ( QMouseEvent * event_n )
{
  //::std::cout << "DS_Slider::mousePressEvent" << "\n";

  QWidget::mousePressEvent ( event_n );

  if ( event_n->button () == Qt::LeftButton ) {

    if ( _cursor_over_handle ) {
      // Start slider drag
      set_slider_down ( true );
    } else {
      // Select a value on the slider range
      if ( _cursor_select_allowed ) {
        set_slider_down ( true );
        const unsigned int hpos (
            map_tick_pos_to_handle_pos ( event_n->pos ().y () ) );
        if ( set_handle_pos ( hpos ) ) {
          update_value_from_handle_pos ();
        }
      }
    }

    if ( slider_down () ) {
      anim_stop ();
    }

    // After a click selection should be allowed
    if ( hasFocus () ) {
      _cursor_select_allowed = true;
    }
    update_cursor_mode ();
    update ();
  }
}

void
DS_Slider::mouseReleaseEvent ( QMouseEvent * event_n )
{
  //::std::cout << "DS_Slider::mouseReleaseEvent" << "\n";

  QWidget::mouseReleaseEvent ( event_n );

  if ( event_n->button () == Qt::LeftButton ) {
    if ( slider_down () ) {
      finish_handle_manipulation ();
      update ();
    }
  }
}

void
DS_Slider::mouseMoveEvent ( QMouseEvent * event_n )
{
  //::std::cout << "DS_Slider::mouseMoveEvent" << "\n";

  QWidget::mouseMoveEvent ( event_n );

  bool do_update ( false );

  if ( slider_down () ) {
    int delta ( 0 );

    {
      int p_now ( event_n->pos ().y () );
      int p_prev ( _mouse_last.y () );

      if ( p_now != p_prev ) {
        int p_min;
        int p_max;
        if ( p_now > p_prev ) {
          // For moving downwards the cursor positions must be in this range
          p_min = _tick_min;
          p_max = height () - 1;
        } else {
          // For moving upwards the cursor positions must be in this range
          p_min = 0;
          p_max = _tick_max;
        }
        p_now = qBound ( p_min, p_now, p_max );
        p_prev = qBound ( p_min, p_prev, p_max );

        delta = ( p_now - p_prev );
      }
    }

    //::std::cout << "DS_Slider::mouseMoveEvent: Delta " << delta << "\n";
    unsigned int hpos ( _handle_pos );
    if ( delta < 0 ) {
      delta = -delta;
      if ( (unsigned int)delta < hpos ) {
        hpos -= delta;
      } else {
        hpos = 0;
      }
    } else {
      hpos += delta;
    }
    if ( set_handle_pos ( hpos ) ) {
      update_value_from_handle_pos ();
      do_update = true;
    }
  }

  _mouse_last = event_n->pos ();

  // Test if the cursor is over the handle
  if ( update_cursor_mode () ) {
    do_update = true;
  }

  if ( do_update ) {
    update ();
  }
}

void
DS_Slider::wheelEvent ( QWheelEvent * event_n )
{

  QWidget::wheelEvent ( event_n );

  if ( rect ().contains ( event_n->pos () ) ) {
    event_n->accept ();
    setFocus ();

    long delta;
    {
      double amount ( maximum_index () / double ( _wheel_degrees ) );
      amount *= ( event_n->delta () / 8.0 );
      if ( amount > 0 ) {
        delta = ::std::ceil ( amount );
      } else {
        delta = ::std::floor ( amount );
      }
    }
    if ( delta == 0 ) {
      if ( event_n->delta () > 0 ) {
        delta = 1;
      } else {
        delta = -1;
      }
    }

    adjust_current_index ( delta );

  } else {
    event_n->ignore ();
  }
}

void
DS_Slider::keyPressEvent ( QKeyEvent * event_n )
{

  QWidget::keyPressEvent ( event_n );

  long idx_adjust ( 0 );
  switch ( event_n->key () ) {
  case Qt::Key_Left:
    idx_adjust = -_step_single;
    break;
  case Qt::Key_Up:
    idx_adjust = _step_single;
    break;
  case Qt::Key_Right:
    idx_adjust = _step_single;
    break;
  case Qt::Key_Down:
    idx_adjust = -_step_single;
    break;
  case Qt::Key_VolumeDown:
    idx_adjust = -_step_single;
    break;
  case Qt::Key_VolumeUp:
    idx_adjust = _step_single;
    break;
  case Qt::Key_PageUp:
    idx_adjust = _step_page;
    break;
  case Qt::Key_PageDown:
    idx_adjust = -_step_page;
    break;
  default:
    break;
  }
  if ( idx_adjust != 0 ) {
    adjust_current_index ( idx_adjust );
  }
}

void
DS_Slider::keyReleaseEvent ( QKeyEvent * event_n )
{
  QWidget::keyReleaseEvent ( event_n );
}

// Window events

void
DS_Slider::resizeEvent ( QResizeEvent * event_n )
{
  //::std::cout << "DS_Slider::resizeEvent " << this << "\n";
  QWidget::resizeEvent ( event_n );

  anim_stop ();

  if ( ( width () <= 0 ) || ( height () <= 0 ) ) {
    return;
  }

  // Handle rect
  unsigned int handle_height ( width () );
  handle_height += ( handle_height + 1 ) % 2; // Make odd

  _handle_rect.setRect ( 0, _handle_pos, width (), handle_height );

  // Handle range
  _handle_pos_max = 0;
  if ( (unsigned int)height () > handle_height ) {
    _handle_pos_max = ( height () - handle_height );
  }
  update_index_mappers ();

  _tick_min = handle_height / 2;
  _tick_max = _tick_min + ( _handle_pos_max + 1 );

  // Update pixmap config
  _meta_bg.ticks_range_max_idx = _handle_pos_max;
  _meta_bg.ticks_range_start = handle_height / 2;

  // Marker size
  {
    int mww ( _handle_rect.height () / 5 );
    int mhh ( _handle_rect.width () / 7 );
    mww = qMin ( mww, mhh );

    if ( mww < 6 ) {
      mww = 6;
    }
    if ( ( ( _handle_rect.width () - mww ) % 2 ) != 0 ) {
      --mww;
    }

    mhh = mww;
    if ( ( mhh % 2 ) == 0 ) {
      --mhh; // height should be odd
    }
    _marker_size.setWidth ( mww );
    _marker_size.setHeight ( mhh );
  }

  // Update value marker painting position offset
  {
    int off_x ( _handle_rect.width () );
    int off_y ( _handle_rect.height () / 2 );
    off_x -= _marker_size.width ();
    off_y -= _marker_size.height () / 2;
    off_x /= 2;
    _marker_offset[ 0 ] = off_x;
    _marker_offset[ 1 ] = off_y;
  }

  // Update handle position
  _handle_pos_index = map_index_to_handle_pos ( current_index () );
  set_handle_pos ( _handle_pos_index );
  update_cursor_mode ();

  update_pixmaps ();
}

void
DS_Slider::paintEvent ( QPaintEvent * )
{
  if ( _update_pixmaps_pending ) {
    update_pixmaps ();
  }
  if ( _update_painter_pending ) {
    update_painter_utils ();
  }
  _dsi.wait_for_request ();
  if ( !_dsi.images ().ready () ) {
    return;
  }

  QPixmap * pixmap_handle ( 0 );
  QPixmap * pixmap_frame ( 0 );
  QPixmap * pixmap_marker_2 ( 0 );
  QPixmap * pixmap_marker_1 ( 0 );
  bool draw_snap_line ( false );
  QPixmap * pixmap_bg_2 ( 0 );
  QPixmap * pixmap_bg_1 ( 0 );

  // Items setup
  {
    // Local state buffer
    const bool loc_under_mouse ( underMouse () );
    const bool loc_is_enabled ( isEnabled () );
    const bool loc_has_focus ( hasFocus () );
    const bool loc_slider_down ( slider_down () );

    // Slider background setup
    {
      // Top half of the background
      ::dpe::Image & img ( _dsi.images ().img_sets[ 0 ]->image ( 0 ) );
      pixmap_bg_1 = img.convert_to_pixmap ();
    }
    {
      // Bottom half of the background
      ::dpe::Image & img ( _dsi.images ().img_sets[ 0 ]->image ( 1 ) );
      pixmap_bg_2 = img.convert_to_pixmap ();
    }

    // Value marker and snap line setup
    if ( loc_slider_down || _anim_run_snap ) {
      draw_snap_line = true;

      ::dpe::Image & img ( _dsi.images ().img_sets[ 1 ]->image ( 0 ) );
      pixmap_marker_1 = img.convert_to_pixmap ();
    }

    // Value hint marker setup
    if ( _cursor_value_hinting ) {
      ::dpe::Image & img ( _dsi.images ().img_sets[ 1 ]->image ( 1 ) );
      pixmap_marker_2 = img.convert_to_pixmap ();
    }

    // Frame setup
    {
      int idx ( -1 );
      if ( loc_has_focus ) {
        idx = 0;
      } else if ( loc_under_mouse && loc_is_enabled ) {
        idx = 1;
      }
      if ( idx >= 0 ) {
        ::dpe::Image & img ( _dsi.images ().img_sets[ 2 ]->image ( idx ) );
        pixmap_frame = img.convert_to_pixmap ();
      }
    }

    // Handle setup
    {
      unsigned int idx;
      if ( loc_slider_down ) {
        idx = 4;
      } else {
        idx = loc_has_focus ? 1 : 0;
        if ( loc_under_mouse && loc_is_enabled ) {
          idx += 2;
        }
      }
      ::dpe::Image & img ( _dsi.images ().img_sets[ 3 ]->image ( idx ) );
      pixmap_handle = img.convert_to_pixmap ();
    }
  }

  // Painting
  QPainter painter ( this );
  painter.setRenderHint ( QPainter::Antialiasing );

  if ( ( pixmap_bg_1 != 0 ) && ( pixmap_bg_2 != 0 ) ) {
    const int mid_pos ( _handle_pos_index + _handle_rect.height () / 2 );
    {
      const QRect re_img ( 0, 0, width (), mid_pos );
      painter.drawPixmap ( re_img, *pixmap_bg_1, re_img );
    }
    {
      const QRect re_img ( 0, mid_pos, width (), height () - mid_pos );
      painter.drawPixmap ( re_img, *pixmap_bg_2, re_img );
    }
  }

  if ( draw_snap_line ) {
    unsigned int pos_diff;
    if ( _handle_pos > _handle_pos_index ) {
      pos_diff = _handle_pos - _handle_pos_index;
    } else {
      pos_diff = _handle_pos_index - _handle_pos;
    }
    if ( pos_diff > 5 ) {
      const double x_mid ( width () / 2.0 );
      double dx1 ( _marker_size.width () / 5.0 );
      dx1 = qMax ( dx1, 1.5 );
      double yy1 ( map_handle_pos_to_tick_pos ( _handle_pos ) );
      double yy2 ( map_handle_pos_to_tick_pos ( _handle_pos_index ) );
      yy1 += 0.5;
      yy2 += 0.5;
      const double yym ( ( yy1 + yy2 ) / 2.0 );

      const int num_pts ( 6 );
      QPointF pts[ num_pts ] = {QPointF ( x_mid + dx1, yy1 ),
                                QPointF ( x_mid, yym ),
                                QPointF ( x_mid + dx1, yy2 ),
                                QPointF ( x_mid - dx1, yy2 ),
                                QPointF ( x_mid, yym ),
                                QPointF ( x_mid - dx1, yy1 )};
      painter.setPen ( _snap_pen );
      painter.setBrush ( _snap_brush );
      painter.drawPolygon ( pts, num_pts );
    }
  }

  if ( pixmap_marker_1 != 0 ) {
    const QPointF pp ( _marker_offset[ 0 ],
                       _marker_offset[ 1 ] + _handle_pos_index );
    painter.drawPixmap ( pp, *pixmap_marker_1 );
  }

  if ( pixmap_marker_2 != 0 ) {
    const QPointF pp ( _marker_offset[ 0 ],
                       _marker_offset[ 1 ] + _handle_pos_hint );
    painter.drawPixmap ( pp, *pixmap_marker_2 );
  }

  if ( pixmap_frame != 0 ) {
    const QPointF pp ( 0, 0 );
    painter.drawPixmap ( pp, *pixmap_frame );
  }

  if ( pixmap_handle != 0 ) {
    painter.drawPixmap ( _handle_rect.topLeft (), *pixmap_handle );
  }
}

void
DS_Slider::update_pixmaps ()
{
  if ( _dsi.image_alloc () != 0 ) {
    if ( isVisible () ) {
      //::std::cout << "DS_Slider::update_pixmaps " << this << "\n";
      _update_pixmaps_pending = false;
      _dsi.wait_for_request ();

      _meta_bg.size = size ();
      _meta_marker.size = _marker_size;
      _meta_frame.size = size ();
      _meta_handle.size = _handle_rect.size ();
      _dsi.set_images_style_sub_id ( _dsi.style_sub_id ( this ) );

      _dsi.send_request ();

      update ();
    } else {
      _update_pixmaps_pending = true;
    }
  }
}

void
DS_Slider::update_painter_utils ()
{
  QColor col;
  {
    if ( wdg_style_db () != 0 ) {
      setPalette ( wdg_style_db ()->palette ( style_id () ) );
    }
    QColor col_bg ( palette ().color ( QPalette::Window ) );
    QColor col_fg ( palette ().color ( QPalette::WindowText ) );
    col = ::Wdg::col_mix ( col_bg, col_fg, 1, 2 );
  }
  col.setAlpha ( 70 );

  _snap_pen.setStyle ( Qt::NoPen );
  _snap_pen.setColor ( col );

  _snap_brush.setStyle ( Qt::SolidPattern );
  _snap_brush.setColor ( col );
}

void
DS_Slider::anim_stop ()
{
  _anim_run_snap = false;
  _anim_timer.stop ();
}

void
DS_Slider::anim_tick ()
{
  bool do_update ( false );

  if ( _anim_run_snap ) {
    // Passed seconds since the animation start
    unsigned int msec ( ::std::abs ( _anim_snap_time.elapsed () ) );
    if ( msec >= _anim_snap_msec_max ) {
      msec = _anim_snap_msec_max;
      _anim_run_snap = false;
    }
    if ( anim_snap_tick ( msec ) ) {
      do_update = true;
    }
    if ( _handle_pos == _handle_pos_index ) {
      _anim_run_snap = false;
      do_update = true;
    }
  }

  if ( !_anim_run_snap ) {
    anim_stop ();
  }
  if ( do_update ) {
    update_cursor_mode ();
    update ();
  }
}

void
DS_Slider::anim_snap_start ()
{
  if ( ( _handle_pos != _handle_pos_index ) ) {

    // Current speed
    double v_x0 ( 0.0 );

    if ( _anim_run_snap ) {
      // A snapping annimation is already running.
      unsigned int msec ( ::std::abs ( _anim_snap_time.elapsed () ) );
      if ( msec < _anim_snap_msec_max ) {
        // Acquire the current speed
        v_x0 = double ( msec ) / 1000.0;
        v_x0 = _cubic_curve.eval_speed ( v_x0 );
      } else {
        msec = _anim_snap_msec_max;
      }
      // Do a anim tick
      anim_snap_tick ( msec );
    } else {
      _anim_run_snap = true;
    }

    _anim_snap_time.start ();
    if ( !_anim_timer.isActive () ) {
      _anim_timer.start ();
    }

    if ( _handle_pos_max > 0 ) {
      unsigned int pos_diff;
      if ( _handle_pos > _handle_pos_index ) {
        pos_diff = _handle_pos - _handle_pos_index;
      } else {
        pos_diff = _handle_pos_index - _handle_pos;
      }

      _anim_snap_msec_max = ( 800 * pos_diff ) / _handle_pos_max;
    } else {
      _anim_snap_msec_max = 500;
    }
    _anim_snap_msec_max = qBound ( 100, (int)_anim_snap_msec_max, 600 );

    _cubic_curve.set_params (
        _handle_pos, v_x0, _handle_pos_index, _anim_snap_msec_max / 1000.0 );
  }
}

bool
DS_Slider::anim_snap_tick ( unsigned int msec_n )
{
  unsigned int hpos ( _handle_pos_index );
  if ( msec_n <= _anim_snap_msec_max ) {
    // Calculate the position from the easing curve
    double dpos ( double ( msec_n ) / 1000.0 );
    dpos = _cubic_curve.eval ( dpos );
    dpos = std::floor ( dpos + 0.5 ); // Round
    if ( dpos > 0.0 ) {
      hpos = static_cast< unsigned int > ( dpos );
    } else {
      hpos = 0;
    }
  }

  return set_handle_pos ( hpos );
}

} // namespace Wdg
