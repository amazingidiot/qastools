/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "switches_pad_widgets_group.hpp"
#include <QEvent>
#include <QFocusEvent>
#include <QPainter>
#include <iostream>

namespace Wdg
{

Switches_Pad_Widgets_Group::Switches_Pad_Widgets_Group ( QWidget * parent_n )
: QWidget ( parent_n )
, _label ( 0 )
, _lay_grid ( 0 )
{
  setContextMenuPolicy ( Qt::NoContextMenu );

  _stem_pen.setWidth ( 1 );

  QVBoxLayout * lay_vbox ( new QVBoxLayout );
  lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
  setLayout ( lay_vbox );
}

Switches_Pad_Widgets_Group::~Switches_Pad_Widgets_Group ()
{
  clear_widgets ();
}

void
Switches_Pad_Widgets_Group::clear_widgets ()
{
  if ( _widgets.size () > 0 ) {
    for ( int ii = 0; ii < _widgets.size (); ++ii ) {
      delete _widgets[ ii ];
    }
    _widgets.clear ();
  }
  if ( _label != 0 ) {
    delete _label;
    _label = 0;
  }
  if ( _lay_grid != 0 ) {
    delete _lay_grid;
    _lay_grid = 0;
  }
}

void
Switches_Pad_Widgets_Group::set_label ( QLabel * wdg_n )
{
  if ( _label != 0 ) {
    delete _label;
  }

  _label = wdg_n;

  if ( _label != 0 ) {
    if ( _lay_grid == 0 ) {
      _lay_grid = new QGridLayout;
      _lay_grid->setContentsMargins ( enum_spacing () * 2, 0, 0, 0 );
      _lay_grid->setHorizontalSpacing ( enum_spacing () );
    }
    lay_vbox ()->addWidget ( _label );
    lay_vbox ()->addLayout ( _lay_grid );
  } else {
    if ( _lay_grid != 0 ) {
      delete _lay_grid;
      _lay_grid = 0;
    }
  }
}

void
Switches_Pad_Widgets_Group::append_widgets ( Switches_Pad_Widgets * spw_n )
{
  if ( spw_n == 0 ) {
    return;
  }
  if ( spw_n->input_wdg () == 0 ) {
    return;
  }

  if ( _lay_grid != 0 ) {

    // Multiple input widgets

    const unsigned int row ( _widgets.size () );
    if ( spw_n->label_wdg () != 0 ) {
      // Input widget with separate label
      _lay_grid->addWidget ( spw_n->input_wdg (), row, 0 );
      _lay_grid->addWidget ( spw_n->label_wdg (), row, 1 );
    } else {
      // Single input widget
      _lay_grid->addWidget ( spw_n->input_wdg (), row, 0 );
    }

  } else {

    // Single input widget

    if ( spw_n->label_wdg () != 0 ) {
      // Input widget with separate label
      QHBoxLayout * lay_hbox ( new QHBoxLayout );
      lay_hbox->setContentsMargins ( 0, 0, 0, 0 );
      lay_hbox->setSpacing ( enum_spacing () );

      lay_hbox->addWidget ( spw_n->input_wdg () );
      lay_hbox->addWidget ( spw_n->label_wdg () );

      lay_vbox ()->addLayout ( lay_hbox );
    } else {
      // Single input widget
      lay_vbox ()->addWidget ( spw_n->input_wdg () );
    }
  }

  _widgets.append ( spw_n );
}

int
Switches_Pad_Widgets_Group::enum_spacing () const
{
  return fontMetrics ().averageCharWidth () * 4 / 3;
}

bool
Switches_Pad_Widgets_Group::eventFilter ( QObject * watched_n,
                                          QEvent * event_n )
{
  bool res ( false );

  if ( ( watched_n == _label ) && ( _label != 0 ) ) {
    if ( event_n->type () == QEvent::FocusIn ) {
      if ( _widgets.size () > 0 ) {
        _widgets[ 0 ]->input_wdg ()->setFocus ();
        res = true;
      }
    }
  }

  return res;
}

void
Switches_Pad_Widgets_Group::paintEvent ( QPaintEvent * event_n )
{
  QWidget::paintEvent ( event_n );

  if ( ( _lay_grid != 0 ) && ( _label != 0 ) && ( num_widgets () > 0 ) ) {
    QRect re_lbl ( _label->geometry () );
    QRect re_wdg ( _lay_grid->itemAt ( 0 )->geometry () );

    int x0 = ( re_lbl.x () * 4 + re_wdg.x () * 3 ) / 7;
    int x1 = re_wdg.x () - 3;

    if ( !re_lbl.isValid () || !re_wdg.isValid () || ( x1 <= x0 ) ) {
      return;
    }

    int y0 = re_lbl.y () + re_lbl.height ();
    int y1 = y0;

    QPainter pnt ( this );
    pnt.setBrush ( Qt::NoBrush );
    pnt.setPen ( _stem_pen );
    for ( unsigned int ii = 0; ii < num_widgets (); ++ii ) {
      re_wdg = _lay_grid->itemAt ( ii )->geometry ();
      int yy ( re_wdg.y () + ( re_wdg.height () + 1 ) / 2 );
      if ( y1 < yy ) {
        y1 = yy;
      }
      pnt.drawLine ( x0, yy, x1, yy );
    }
    pnt.drawLine ( x0, y0, x0, y1 );
  }
}

} // namespace Wdg
