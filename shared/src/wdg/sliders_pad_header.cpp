/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "sliders_pad_header.hpp"
#include "wdg/sliders_pad_data.hpp"
#include "wdg/sliders_pad_style.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QPicture>
#include <cmath>
#include <iostream>

namespace Wdg
{

Sliders_Pad_Header::Sliders_Pad_Header ( Sliders_Pad_Data * sp_data_n,
                                         Sliders_Pad_Style * sp_style_n,
                                         QWidget * parent_n )
: QWidget ( parent_n )
, _sp_data ( sp_data_n )
, _sp_style ( sp_style_n )
, _focus_idx ( ~0 )
, _weak_focus_idx ( ~0 )
, _invalid_idx ( ~0 )
{
  hd_data ().max_str_length_px = fontMetrics ().averageCharWidth () * 18;
  hd_data ().widget = this;

  setMouseTracking ( true );

  int max_h ( hd_data ().spacing_vertical );
  max_h += hd_data ().pad_left;
  max_h += hd_data ().pad_right;
  max_h += hd_data ().max_str_length_px;
  setMaximumHeight ( max_h );

  update_painter_states ();
}

Sliders_Pad_Header::~Sliders_Pad_Header () {}

QSize
Sliders_Pad_Header::minimumSizeHint () const
{
  QSize res ( 64, 0 );
  res.rheight () += hd_data ().spacing_vertical;
  res.rheight () += fontMetrics ().height () * 5 / 2;

  return res;
}

QSize
Sliders_Pad_Header::sizeHint () const
{
  return minimumSizeHint ();
}

unsigned int
Sliders_Pad_Header::label_str_length_px_max ( const QString & str_n ) const
{
  unsigned int res;
  res = ::std::ceil ( fontMetrics ().width ( str_n ) );
  res = qMin ( res, hd_data ().max_str_length_px );
  return res;
}

void
Sliders_Pad_Header::set_focus_idx ( unsigned int group_idx_n,
                                    unsigned int column_idx_n )
{
  unsigned int idx ( _invalid_idx );
  if ( group_idx_n < sp_data ()->groups.size () ) {
    const Sliders_Pad_Data_Group * sp_grp ( sp_data ()->groups[ group_idx_n ] );
    if ( hd_data ().column_labels ) {
      if ( column_idx_n < sp_grp->columns.size () ) {
        const Sliders_Pad_Data_Column * sp_col (
            sp_grp->columns[ column_idx_n ] );
        idx = sp_col->col_total_idx;
      }
    } else {
      idx = group_idx_n;
    }

    if ( idx >= hd_data ().labels.size () ) {
      idx = _invalid_idx;
    }
  }
  _focus_idx = idx;
}

void
Sliders_Pad_Header::set_label_text ( unsigned int lbl_idx_n,
                                     const QString & txt_n )
{
  if ( lbl_idx_n < hd_data ().labels.size () ) {
    Sliders_Pad_Header_Label & lbl ( hd_data ().labels[ lbl_idx_n ] );
    lbl.text = txt_n;
    elided_label_text ( lbl );
    update ( lbl.label_txt_bbox );
  }
}

void
Sliders_Pad_Header::enterEvent ( QEvent * )
{
  if ( _weak_focus_idx < hd_data ().labels.size () ) {
    _weak_focus_idx = _invalid_idx;
    update ();
  }
}

void
Sliders_Pad_Header::leaveEvent ( QEvent * )
{
  if ( _weak_focus_idx < hd_data ().labels.size () ) {
    _weak_focus_idx = _invalid_idx;
    update ();
  }
}

void
Sliders_Pad_Header::mousePressEvent ( QMouseEvent * event_n )
{
  const Sliders_Pad_Header_Label * lbl ( find_label ( event_n->pos () ) );
  if ( lbl != 0 ) {
    emit sig_label_selected ( lbl->group_idx, lbl->column_idx );
  }
}

void
Sliders_Pad_Header::mouseMoveEvent ( QMouseEvent * event_n )
{
  const Sliders_Pad_Header_Label * lbl ( find_label ( event_n->pos () ) );

  bool changed ( false );
  {
    unsigned int idx ( _invalid_idx );
    if ( lbl != 0 ) {
      idx = lbl->column_total_idx;
    }
    if ( idx != _weak_focus_idx ) {
      _weak_focus_idx = idx;
      changed = true;
    }
  }

  if ( changed ) {
    Qt::CursorShape cursor_shape ( Qt::ArrowCursor );
    if ( lbl == 0 ) {
      setToolTip ( QString () );
    } else {
      setToolTip ( lbl->tool_tip );
      if ( ( event_n->buttons () & Qt::LeftButton ) != 0 ) {
        if ( hd_data ().label_sliding ) {
          emit sig_label_selected ( lbl->group_idx, lbl->column_idx );
        }
      }
      cursor_shape = Qt::PointingHandCursor;
    }

    {
      // Adjust the cursor
      QCursor cursor_new ( cursor () );
      if ( cursor_new.shape () != cursor_shape ) {
        cursor_new.setShape ( cursor_shape );
        setCursor ( cursor_new );
      }
    }
    update ();
  }
}

void
Sliders_Pad_Header::changeEvent ( QEvent * event_n )
{
  QWidget::changeEvent ( event_n );
  update_painter_states ();
}

void
Sliders_Pad_Header::paintEvent ( QPaintEvent * event_n )
{
  //::std::cout << "Sliders_Pad_Header::paintEvent" << "\n";
  QWidget::paintEvent ( event_n );

  // Update elided text strings on demand
  if ( hd_data ().update_elided_texts ) {
    hd_data ().update_elided_texts = false;
    update_elided_texts ();
  }

  if ( hd_data ().update_decoration ) {
    hd_data ().update_decoration = false;
    if ( hd_data ().upside_down ) {
      sp_style ()->paint_footer_decoration ();
    } else {
      sp_style ()->paint_header_decoration ();
    }
  }

  QPainter pnt ( this );
  pnt.setRenderHints ( QPainter::Antialiasing | QPainter::TextAntialiasing |
                       QPainter::SmoothPixmapTransform );

  // Draw debug area
  {
    // painter.setBrush ( hd_data().upside_down ? Qt::cyan : Qt::yellow );
    // painter.setPen ( Qt::NoPen );
    // painter.drawRect ( rect() );
  }

  if ( hd_data ().labels.size () > 0 ) {
    paint_label_rects ( pnt );
    paint_label_decos ( pnt );
    paint_label_texts ( pnt );
  }
}

void
Sliders_Pad_Header::paint_label_rects ( QPainter & pnt_n )
{
  pnt_n.setPen ( Qt::NoPen );

  if ( _lbl_rect_brush[ 0 ].style () != Qt::NoBrush ) {
    pnt_n.setBrush ( _lbl_rect_brush[ 0 ] );
    for ( unsigned int lii = 0; lii < hd_data ().labels.size (); ++lii ) {
      // Draw focus items later
      if ( ( lii != _focus_idx ) && ( lii != _weak_focus_idx ) ) {
        const Sliders_Pad_Header_Label & lbl ( hd_data ().labels[ lii ] );
        pnt_n.setTransform ( lbl.label_trans );
        pnt_n.drawRect ( lbl.label_rect );
      }
    }
  }

  if ( ( _focus_idx < hd_data ().labels.size () ) &&
       ( _lbl_rect_brush[ 1 ].style () != Qt::NoBrush ) ) {
    pnt_n.setBrush ( _lbl_rect_brush[ 1 ] );
    const Sliders_Pad_Header_Label & lbl ( hd_data ().labels[ _focus_idx ] );
    pnt_n.setTransform ( lbl.label_trans );
    pnt_n.drawRect ( lbl.label_rect );
  }

  if ( ( _weak_focus_idx < hd_data ().labels.size () ) &&
       ( _lbl_rect_brush[ 2 ].style () != Qt::NoBrush ) ) {
    pnt_n.setBrush ( _lbl_rect_brush[ 2 ] );
    const Sliders_Pad_Header_Label & lbl (
        hd_data ().labels[ _weak_focus_idx ] );
    pnt_n.setTransform ( lbl.label_trans );
    pnt_n.drawRect ( lbl.label_rect );
  }
}

void
Sliders_Pad_Header::paint_label_decos ( QPainter & pnt_n )
{
  pnt_n.setPen ( Qt::NoPen );
  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.resetTransform ();

  // Draw groups/columns decoration graphics
  const unsigned int pic_idx ( hd_data ().upside_down ? 1 : 0 );
  for ( unsigned int gii = 0; gii < sp_data ()->groups.size (); ++gii ) {
    // Group picture
    const Sliders_Pad_Data_Group * sp_grp ( sp_data ()->groups[ gii ] );
    pnt_n.drawPicture ( 0, 0, sp_grp->hd_pics[ pic_idx ] );

    // Columns pictures
    for ( unsigned int cii = 0; cii < sp_grp->columns.size (); ++cii ) {
      const Sliders_Pad_Data_Column * sp_col ( sp_grp->columns[ cii ] );
      pnt_n.drawPicture ( 0, 0, sp_col->hd_pics[ pic_idx ] );
    }
  }
}

void
Sliders_Pad_Header::paint_label_texts ( QPainter & pnt_n )
{
  Qt::Alignment txt_align ( Qt::AlignLeft | Qt::AlignVCenter );
  if ( hd_data ().upside_down ) {
    txt_align = Qt::AlignRight | Qt::AlignVCenter;
  }

  pnt_n.setPen ( _lbl_txt_pen[ 0 ] );
  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setFont ( _lbl_txt_font[ 0 ] );

  for ( unsigned int lii = 0; lii < hd_data ().labels.size (); ++lii ) {
    // Draw focus items later
    if ( ( lii != _focus_idx ) && ( lii != _weak_focus_idx ) ) {
      const Sliders_Pad_Header_Label & lbl ( hd_data ().labels[ lii ] );

      { // Pen color
        const QPen & ppen ( pnt_n.pen () );
        if ( lbl.col_fg != ppen.color () ) {
          QPen npen ( ppen );
          npen.setColor ( lbl.col_fg );
          pnt_n.setPen ( npen );
        }
      }

      // Draw text
      pnt_n.setTransform ( lbl.label_trans );
      pnt_n.drawText ( lbl.text_rect, txt_align, lbl.text_elided );
    }
  }

  if ( _weak_focus_idx != _focus_idx ) {
    paint_label_text ( pnt_n, txt_align, _focus_idx, 1 );
    paint_label_text ( pnt_n, txt_align, _weak_focus_idx, 2 );
  } else {
    paint_label_text ( pnt_n, txt_align, _focus_idx, 3 );
  }
}

void
Sliders_Pad_Header::paint_label_text ( QPainter & pnt_n,
                                       Qt::Alignment txt_align_n,
                                       unsigned int lbl_idx_n,
                                       unsigned int state_idx_n )
{
  if ( lbl_idx_n < hd_data ().labels.size () ) {
    pnt_n.setFont ( _lbl_txt_font[ state_idx_n ] );
    pnt_n.setPen ( _lbl_txt_pen[ state_idx_n ] );

    const Sliders_Pad_Header_Label & lbl ( hd_data ().labels[ lbl_idx_n ] );
    pnt_n.setTransform ( lbl.label_trans );
    pnt_n.drawText ( lbl.text_rect, txt_align_n, lbl.text_elided );
  }
}

void
Sliders_Pad_Header::update_painter_states ()
{
  _lbl_rect_brush[ 0 ].setStyle ( Qt::NoBrush );
  _lbl_rect_brush[ 1 ].setStyle ( Qt::SolidPattern );
  if ( hd_data ().upside_down ) {
    _lbl_rect_brush[ 1 ].setColor ( palette ().color ( QPalette::Button ) );
  } else {
    _lbl_rect_brush[ 1 ].setColor ( palette ().color ( QPalette::Highlight ) );
  }
  _lbl_rect_brush[ 2 ].setStyle ( Qt::NoBrush );

  const QFont fnt ( font () );
  _lbl_txt_font[ 0 ] = fnt;
  _lbl_txt_font[ 1 ] = fnt;
  _lbl_txt_font[ 2 ] = fnt;
  _lbl_txt_font[ 2 ].setUnderline ( !fnt.underline () );
  _lbl_txt_font[ 3 ] = _lbl_txt_font[ 2 ];

  const QPen pen;
  _lbl_txt_pen[ 0 ] = pen;
  _lbl_txt_pen[ 1 ] = pen;
  if ( hd_data ().upside_down ) {
    _lbl_txt_pen[ 1 ].setColor ( palette ().color ( QPalette::ButtonText ) );
  } else {
    _lbl_txt_pen[ 1 ].setColor (
        palette ().color ( QPalette::HighlightedText ) );
  }
  _lbl_txt_pen[ 2 ] = pen;
  _lbl_txt_pen[ 3 ] = _lbl_txt_pen[ 1 ];
}

const Sliders_Pad_Header_Label *
Sliders_Pad_Header::find_label ( const QPoint & pos_n )
{
  const Sliders_Pad_Header_Label * res ( 0 );

  const QPointF pos_f ( pos_n );
  const unsigned int num_lbl ( hd_data ().labels.size () );
  unsigned int lbl_idx ( num_lbl );
  for ( unsigned int lii = 0; lii < num_lbl; ++lii ) {
    --lbl_idx;

    const Sliders_Pad_Header_Label & lbl ( hd_data ().labels[ lbl_idx ] );
    const Sliders_Pad_Data_Group * sp_grp (
        sp_data ()->groups[ lbl.group_idx ] );

    if ( pos_n.x () >= (int)sp_grp->group_pos ) {
      const QPointF pos_map ( lbl.label_trans_inv.map ( pos_f ) );
      QRectF rect_f ( lbl.label_rect );
      rect_f.adjust ( 0, -1.5, 0, 3.0 );
      if ( rect_f.contains ( pos_map ) ) {
        res = &lbl;
        break;
      }
    }
  }

  return res;
}

void
Sliders_Pad_Header::elided_label_text ( Sliders_Pad_Header_Label & lbl_n )
{
  const QFontMetrics & fmet ( fontMetrics () );
  lbl_n.text_elided =
      fmet.elidedText ( lbl_n.text, Qt::ElideRight, lbl_n.text_area.width () );

  // Update text rectangle
  lbl_n.text_rect = lbl_n.text_area;
  if ( lbl_n.text_rect.width () > lbl_n.label_length_max ) {
    lbl_n.text_rect.setWidth ( lbl_n.label_length_max );
  }
}

void
Sliders_Pad_Header::update_elided_texts ()
{
  for ( unsigned int ii = 0; ii < hd_data ().labels.size (); ++ii ) {
    elided_label_text ( hd_data ().labels[ ii ] );
  }
}

} // namespace Wdg
