/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "balloon_widget.hpp"
#include <QApplication>
#include <QBitmap>
#include <QDesktopWidget>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <iostream>

namespace Wdg
{

Balloon_Widget::Balloon_Widget ( QWidget * parent_n )
: QWidget ( parent_n, Qt::ToolTip )
, _update_pxmap ( true )
, _stay_on_hover ( true )
, _remains_on_hover ( false )
{
  setFocusPolicy ( Qt::NoFocus );

  _close_timer.setInterval ( 6000 );
  _close_timer.setSingleShot ( true );
  connect (
      &_close_timer, SIGNAL ( timeout () ), this, SLOT ( close_timeout () ) );

  hide ();
  {
    int mg_h ( fontMetrics ().height () );
    int mg_v ( mg_h * 3 / 4 );
    setContentsMargins ( mg_h, mg_v, mg_h, mg_v );
  }

  {
    QVBoxLayout * lay_vbox ( new QVBoxLayout );
    lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
    setLayout ( lay_vbox );
  }
}

void
Balloon_Widget::add_widget ( QWidget * wdg_n )
{
  layout ()->addWidget ( wdg_n );
}

unsigned int
Balloon_Widget::duration_ms () const
{
  return qMax ( 0, _close_timer.interval () );
}

void
Balloon_Widget::set_duration_ms ( unsigned int ms_n )
{
  _close_timer.setInterval ( ms_n );
}

void
Balloon_Widget::set_stay_on_hover ( bool flag_n )
{
  _stay_on_hover = flag_n;
}

void
Balloon_Widget::set_tray_icon_geometry ( const QRect & geom_n )
{
  if ( _tray_icon_geom != geom_n ) {
    _tray_icon_geom = geom_n;
    update_geometry ();
  }
}

void
Balloon_Widget::update_geometry ()
{
  if ( !_tray_icon_geom.isValid () ) {
    return;
  }

  const QPoint ti_tl ( _tray_icon_geom.topLeft () );

  QRect srect;
  QRect arect;
  {
    QDesktopWidget * desk_wdg ( QApplication::desktop () );
    srect = desk_wdg->screenGeometry ( ti_tl );
    arect = desk_wdg->availableGeometry ( ti_tl );
  }

  QSize nsize ( minimumSizeHint () );
  QPoint npos;

  unsigned int mg_hor ( fontMetrics ().averageCharWidth () );
  unsigned int mg_vert ( fontMetrics ().height () / 2 );

  // Find corner
  if ( ti_tl.x () < ( srect.left () + srect.width () - ti_tl.x () ) ) {
    // Left
    npos.setX ( arect.left () + mg_hor );
  } else {
    // Right
    npos.setX ( arect.left () + arect.width () - nsize.width () - mg_hor );
  }

  if ( ti_tl.y () < ( srect.top () + srect.height () - ti_tl.y () ) ) {
    // Top
    npos.setY ( arect.top () + mg_vert );
  } else {
    // Bottom
    npos.setY ( arect.top () + arect.height () - nsize.height () - mg_vert );
  }

  move ( npos );
  resize ( nsize );
}

void
Balloon_Widget::update_mask ()
{
  QBitmap mask ( size () );
  mask.fill ( Qt::color0 );

  {
    QPainter pnt ( &mask );
    pnt.setRenderHints ( QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform );
    pnt.setPen ( Qt::NoPen );
    pnt.setBrush ( QBrush ( Qt::color1 ) );
    {
      QRectF rectf ( rect () );
      pnt.drawRoundedRect ( rectf, _corner_rad, _corner_rad );
    }
  }

  setMask ( mask );
}

void
Balloon_Widget::update_pixmap ()
{
  QImage img ( size (), QImage::Format_ARGB32_Premultiplied );
  img.fill ( 0 );

  {
    QPainter pnt ( &img );
    pnt.setRenderHints ( QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform );

    {
      QPen pen ( palette ().color ( QPalette::WindowText ), _border_width );
      pnt.setPen ( pen );
      QBrush brush ( palette ().color ( QPalette::Window ) );
      pnt.setBrush ( brush );
    }

    {
      QRectF rectf ( rect () );
      const double adj ( _border_width / 2.0 );
      rectf.adjust ( adj, adj, -adj, -adj );
      pnt.drawRoundedRect ( rectf, _corner_rad, _corner_rad );
    }
  }

  _pxmap = QPixmap::fromImage ( img );
}

void
Balloon_Widget::start_show ()
{
  if ( _tray_icon_geom.isValid () ) {
    _remains_on_hover = false;
    show ();
    _close_timer.start ();
  }
}

void
Balloon_Widget::close_timeout ()
{
  if ( _stay_on_hover ) {
    if ( underMouse () ) {
      _remains_on_hover = true;
    }
  }

  if ( !_remains_on_hover ) {
    emit sig_close ();
  }
}

void
Balloon_Widget::leaveEvent ( QEvent * event_n )
{
  QWidget::leaveEvent ( event_n );
  if ( _remains_on_hover ) {
    _remains_on_hover = false;
    emit sig_close ();
  }
}

void
Balloon_Widget::resizeEvent ( QResizeEvent * event_n )
{
  _update_pxmap = true;
  QWidget::resizeEvent ( event_n );
}

void
Balloon_Widget::paintEvent ( QPaintEvent * )
{
  if ( _update_pxmap ) {
    _update_pxmap = false;
    update_mask ();
    update_pixmap ();
  }

  {
    QPainter pnt ( this );
    pnt.drawPixmap ( QPoint ( 0, 0 ), _pxmap );
  }
}

} // namespace Wdg
