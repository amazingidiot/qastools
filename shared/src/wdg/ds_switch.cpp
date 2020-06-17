/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_switch.hpp"
#include "dpe/image_allocator.hpp"
#include "dpe/image_request.hpp"
#include "dpe/image_set.hpp"
#include "wdg/ds_widget_types.hpp"
#include <QEvent>
#include <QPainter>
#include <iostream>

namespace Wdg
{

DS_Switch::DS_Switch ( QWidget * parent_n, ::dpe::Image_Allocator * alloc_n )
: QAbstractButton ( parent_n )
, _dsi ( 2, alloc_n )
, _update_pixmaps_pending ( false )
, _meta_bg ( num_images_bg, ::Wdg::DS_SWITCH, 0 )
, _meta_handle ( num_images_handle, ::Wdg::DS_SWITCH, 1 )
{
  _dsi.image_request ()->meta[ 0 ] = &_meta_bg;
  _dsi.image_request ()->meta[ 1 ] = &_meta_handle;

  setCheckable ( true );
  {
    QSizePolicy policy ( sizePolicy () );
    policy.setHorizontalPolicy ( QSizePolicy::Preferred );
    policy.setVerticalPolicy ( QSizePolicy::Preferred );
    setSizePolicy ( policy );
  }
}

DS_Switch::~DS_Switch ()
{
  _dsi.set_image_alloc ( 0 );
}

QSize
DS_Switch::sizeHint () const
{
  ensurePolished ();
  const QFontMetrics fmet ( fontMetrics () );
  return QSize ( fmet.height (), fmet.height () );
}

QSize
DS_Switch::minimumSizeHint () const
{
  ensurePolished ();
  const QFontMetrics fmet ( fontMetrics () );
  return QSize ( fmet.height (), fmet.height () );
}

void
DS_Switch::set_image_alloc ( ::dpe::Image_Allocator * alloc_n )
{
  _dsi.set_image_alloc ( alloc_n );
}

void
DS_Switch::set_variant_id ( unsigned int id_n )
{
  _dsi.set_images_variant_id ( id_n );
}

void
DS_Switch::set_style_id ( unsigned int id_n )
{
  _dsi.set_images_style_id ( id_n );
}

// Events

void
DS_Switch::changeEvent ( QEvent * event_n )
{
  QWidget::changeEvent ( event_n );

  if ( ( event_n->type () == QEvent::ActivationChange ) ||
       ( event_n->type () == QEvent::EnabledChange ) ||
       ( event_n->type () == QEvent::StyleChange ) ||
       ( event_n->type () == QEvent::PaletteChange ) ||
       ( event_n->type () == QEvent::LayoutDirectionChange ) ) {
    update_pixmaps ();
  }
}

void
DS_Switch::enterEvent ( QEvent * event_n )
{
  QAbstractButton::enterEvent ( event_n );
  update ();
}

void
DS_Switch::leaveEvent ( QEvent * event_n )
{
  QAbstractButton::leaveEvent ( event_n );
  update ();
}

void
DS_Switch::resizeEvent ( QResizeEvent * event_n )
{
  QAbstractButton::resizeEvent ( event_n );
  QRect ren ( contentsRect () );
  if ( ren.width () != ren.height () ) {
    const int delta ( ren.width () - ren.height () );
    if ( delta > 0 ) {
      ren.setWidth ( ren.height () );
      ren.moveLeft ( ren.left () + delta / 2 );
    } else {
      ren.setHeight ( ren.width () );
      ren.moveTop ( ren.top () - delta / 2 );
    }
  }

  _pxmap_rect = ren;

  update_pixmaps ();
}

void
DS_Switch::paintEvent ( QPaintEvent * )
{
  if ( _update_pixmaps_pending ) {
    update_pixmaps ();
  }
  _dsi.wait_for_request ();
  if ( !_dsi.images ().ready () ) {
    return;
  }

  QPixmap * pixmap_handle ( 0 );
  QPixmap * pixmap_bg ( 0 );

  // Background setup
  {
    unsigned int img_idx ( 0 );
    if ( hasFocus () ) {
      img_idx = 1;
    }
    if ( underMouse () && isEnabled () ) {
      img_idx += 2;
    }

    ::dpe::Image & img ( _dsi.images ().img_sets[ 0 ]->image ( img_idx ) );
    pixmap_bg = img.convert_to_pixmap ();
  }

  // Handle setup
  {
    unsigned int img_idx ( 0 );
    if ( isDown () ) {
      img_idx = 8;
      if ( underMouse () ) {
        ++img_idx;
      }
    } else {
      if ( isChecked () ) {
        img_idx = 4;
      }
      if ( underMouse () && isEnabled () ) {
        img_idx += 2;
      }
      if ( hasFocus () ) {
        ++img_idx;
      }
    }

    ::dpe::Image & img ( _dsi.images ().img_sets[ 1 ]->image ( img_idx ) );
    pixmap_handle = img.convert_to_pixmap ();
  }

  // Painting
  QPainter painter ( this );
  if ( pixmap_bg != 0 ) {
    painter.drawPixmap ( _pxmap_rect.topLeft (), *pixmap_bg );
  }
  if ( pixmap_handle != 0 ) {
    painter.drawPixmap ( _pxmap_rect.topLeft (), *pixmap_handle );
  }
}

void
DS_Switch::update_pixmaps ()
{
  if ( _dsi.image_alloc () != 0 ) {
    if ( isVisible () ) {
      _update_pixmaps_pending = false;
      //::std::cout << "DS_Slider::update_pixmaps " << this << "\n";
      _dsi.wait_for_request ();

      _meta_bg.size = _pxmap_rect.size ();
      _meta_handle.size = _pxmap_rect.size ();
      _dsi.set_images_style_sub_id ( _dsi.style_sub_id ( this ) );

      _dsi.send_request ();

      update ();
    } else {
      _update_pixmaps_pending = true;
    }
  }
}

} // namespace Wdg
