/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "sliders_pad.hpp"
#include "wdg/color_methods.hpp"
#include "wdg/ds_slider.hpp"
#include "wdg/ds_switch.hpp"
#include "wdg/ds_widget_style_db.hpp"
#include "wdg/event_types.hpp"
#include "wdg/pad_proxies_column.hpp"
#include "wdg/pad_proxies_group.hpp"
#include "wdg/pad_proxy_slider.hpp"
#include "wdg/pad_proxy_style.hpp"
#include "wdg/pad_proxy_switch.hpp"
#include "wdg/pass_events.hpp"
#include "wdg/sliders_pad_data.hpp"
#include "wdg/sliders_pad_footer.hpp"
#include "wdg/sliders_pad_header.hpp"
#include "wdg/sliders_pad_layout.hpp"
#include "wdg/sliders_pad_style.hpp"
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QPainter>
#include <iostream>

namespace Wdg
{

Sliders_Pad::Sliders_Pad ( QWidget * parent_n,
                           ::dpe::Image_Allocator * isg_alloc_n )
: QWidget ( parent_n )
, _sp_data ( new Sliders_Pad_Data ( this ) )
, _update_decoration ( true )
, _wheel_degrees ( 720 )
, _sp_style ( new Sliders_Pad_Style ( _sp_data ) )
, _wdg_style_db ( 0 )
, _image_alloc ( isg_alloc_n )
{
  setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );

  {
    Sliders_Pad_Header * lheader (
        new Sliders_Pad_Header ( _sp_data, _sp_style, this ) );

    Sliders_Pad_Footer * lfooter (
        new Sliders_Pad_Footer ( _sp_data, _sp_style, this ) );

    _sp_data->header = lheader;
    _sp_data->footer = lfooter;
    _sp_data->header_data = &lheader->hd_data ();
    _sp_data->footer_data = &lfooter->hd_data ();

    connect ( lheader,
              SIGNAL ( sig_label_selected ( unsigned int, unsigned int ) ),
              this,
              SLOT ( header_label_selected ( unsigned int, unsigned int ) ) );

    connect ( lfooter,
              SIGNAL ( sig_label_selected ( unsigned int, unsigned int ) ),
              this,
              SLOT ( footer_label_selected ( unsigned int, unsigned int ) ) );
  }

  update_colors ();
}

Sliders_Pad::~Sliders_Pad ()
{
  clear_proxies_groups ();
  delete _sp_style;
  delete _sp_data;
}

void
Sliders_Pad::set_wdg_style_db ( const ::Wdg::DS_Widget_Style_Db * style_db_n )
{
  if ( _wdg_style_db == style_db_n ) {
    return;
  }

  _wdg_style_db = style_db_n;
}

void
Sliders_Pad::set_image_alloc ( ::dpe::Image_Allocator * alloc_n )
{
  _image_alloc = alloc_n;

  unsigned int num ( num_widgets () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    QWidget * wdg ( widget ( ii ) );

    {
      ::Wdg::DS_Slider * swdg ( dynamic_cast<::Wdg::DS_Slider * > ( wdg ) );
      if ( swdg != 0 ) {
        swdg->set_image_alloc ( image_alloc () );
        continue;
      }
    }

    {
      ::Wdg::DS_Switch * swdg ( dynamic_cast<::Wdg::DS_Switch * > ( wdg ) );
      if ( swdg != 0 ) {
        swdg->set_image_alloc ( image_alloc () );
        continue;
      }
    }
  }
}

QWidget *
Sliders_Pad::header ()
{
  return _sp_data->header;
}

QWidget *
Sliders_Pad::footer ()
{
  return _sp_data->footer;
}

Sliders_Pad_Header *
Sliders_Pad::header_cast ()
{
  return _sp_data->header_cast ();
}

Sliders_Pad_Footer *
Sliders_Pad::footer_cast ()
{
  return _sp_data->footer_cast ();
}

Sliders_Pad_Header_Data *
Sliders_Pad::header_data ()
{
  return _sp_data->header_data;
}

Sliders_Pad_Header_Data *
Sliders_Pad::footer_data ()
{
  return _sp_data->footer_data;
}

// Show footer

bool
Sliders_Pad::footer_visible () const
{
  return _sp_data->show_value_labels;
}

void
Sliders_Pad::set_footer_visible ( bool flag_n )
{
  if ( _sp_data->show_value_labels != flag_n ) {
    _sp_data->show_value_labels = flag_n;
    _sp_data->footer->setVisible ( flag_n );
  }
}

void
Sliders_Pad::set_wheel_degrees ( unsigned int delta_n )
{
  if ( ( delta_n == 0 ) || ( wheel_degrees () == delta_n ) ) {
    return;
  }

  _wheel_degrees = delta_n;

  unsigned int num ( num_widgets () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    ::Wdg::DS_Slider * slider (
        dynamic_cast<::Wdg::DS_Slider * > ( widget ( ii ) ) );
    if ( slider != 0 ) {
      slider->set_wheel_degrees ( wheel_degrees () );
    }
  }
}

void
Sliders_Pad::clear_widgets ()
{
  if ( layout () != 0 ) {
    delete layout ();
  }

  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    ::Wdg::Pad_Proxies_Group * grp ( _proxies_groups[ ii ] );
    grp->set_pad ( 0 );
    for ( unsigned int jj = 0; jj < grp->num_columns (); ++jj ) {
      ::Wdg::Pad_Proxies_Column * col ( grp->column ( jj ) );
      if ( col->slider_proxy () != 0 ) {
        col->slider_proxy ()->set_widget ( 0 );
      }
      if ( col->switch_proxy () != 0 ) {
        col->switch_proxy ()->set_widget ( 0 );
      }
    }
  }

  // Clear widgets
  if ( _widgets.size () > 0 ) {
    for ( int ii = 0; ii < _widgets.size (); ++ii ) {
      delete _widgets[ ii ];
    }
    _widgets.clear ();
  }
  header_data ()->labels.clear ();
  footer_data ()->labels.clear ();

  for ( unsigned int ii = 0; ii < _sp_data->groups.size (); ++ii ) {
    Sliders_Pad_Data_Group * sp_grp ( _sp_data->groups[ ii ] );
    for ( unsigned int jj = 0; jj < sp_grp->columns.size (); ++jj ) {
      delete sp_grp->columns[ jj ];
    }
    sp_grp->columns.clear ();
    delete sp_grp;
  }
  _sp_data->groups.clear ();
}

void
Sliders_Pad::create_widgets ()
{
  const unsigned int num_groups ( _proxies_groups.size () );
  if ( num_groups == 0 ) {
    return;
  }

  if ( layout () != 0 ) {
    delete layout ();
  }

  // Shared data setup
  _sp_data->groups.resize ( num_groups );
  unsigned int num_columns_total ( 0 );
  unsigned int col_total_idx ( 0 );
  for ( unsigned int ii = 0; ii < num_groups; ++ii ) {
    ::Wdg::Pad_Proxies_Group * sppg ( proxies_group ( ii ) );
    ::Wdg::Sliders_Pad_Data_Group * sp_grp (
        new Sliders_Pad_Data_Group ( _sp_data ) );

    sp_grp->sppg = sppg;
    sp_grp->num_sliders = sppg->num_sliders ();
    sp_grp->num_switches = sppg->num_switches ();
    const unsigned int num_cols ( sppg->num_columns () );
    sp_grp->columns.resize ( num_cols );
    for ( unsigned int jj = 0; jj < num_cols; ++jj ) {
      ::Wdg::Pad_Proxies_Column * sppc ( sppg->column ( jj ) );
      ::Wdg::Sliders_Pad_Data_Column * sp_col (
          new Sliders_Pad_Data_Column ( sp_grp ) );

      if ( sppc->show_value_string () && sppc->has_slider () ) {
        connect ( sppc,
                  SIGNAL ( sig_value_string_changed () ),
                  sp_col,
                  SLOT ( update_footer_label () ) );
      }

      sp_col->col_idx = jj;
      sp_col->col_total_idx = col_total_idx;
      sp_col->show_value_label = sppc->show_value_string ();
      sp_col->sppc = sppc;

      sp_grp->columns[ jj ] = sp_col;
      ++col_total_idx;
    }
    _sp_data->groups[ ii ] = sp_grp;
    num_columns_total += num_cols;
  }

  // Header and footer setup
  header_data ()->labels.resize ( num_groups );
  header_data ()->update_elided_texts = true;

  footer_data ()->labels.resize ( num_columns_total );
  footer_data ()->update_elided_texts = true;

  col_total_idx = 0;
  for ( unsigned int ii = 0; ii < num_groups; ++ii ) {
    Pad_Proxies_Group * sppg ( proxies_group ( ii ) );

    // Setup header labels variables
    {
      ::Wdg::Sliders_Pad_Header_Label & lbl ( header_data ()->labels[ ii ] );
      lbl.group_idx = ii;
      lbl.column_idx = 0;
      lbl.column_total_idx = ii;
      lbl.text = sppg->group_name ();
      lbl.tool_tip = sppg->tool_tip ();
      if ( wdg_style_db () != 0 ) {
        lbl.col_fg =
            wdg_style_db ()->color ( sppg->style_id (), QPalette::WindowText );
      } else {
        lbl.col_fg = palette ().color ( QPalette::WindowText );
      }
      lbl.label_length_max =
          header_cast ()->label_str_length_px_max ( sppg->group_name () );
    }

    // Setup footer labels variables
    const unsigned int num_cols ( sppg->num_columns () );
    for ( unsigned int jj = 0; jj < num_cols; ++jj ) {
      ::Wdg::Pad_Proxies_Column * sppc ( sppg->column ( jj ) );
      ::Wdg::Sliders_Pad_Header_Label & lbl (
          footer_data ()->labels[ col_total_idx ] );

      lbl.group_idx = ii;
      lbl.column_idx = jj;
      lbl.column_total_idx = col_total_idx;
      lbl.tool_tip = sppg->tool_tip ();
      if ( wdg_style_db () != 0 ) {
        lbl.col_fg =
            wdg_style_db ()->color ( sppg->style_id (), QPalette::WindowText );
      } else {
        lbl.col_fg = palette ().color ( QPalette::WindowText );
      }
      {
        unsigned int llength[ 2 ] = {0, 0};
        llength[ 0 ] = footer_cast ()->label_str_length_px_max (
            sppc->value_min_string () );
        llength[ 1 ] = footer_cast ()->label_str_length_px_max (
            sppc->value_max_string () );
        lbl.label_length_max = qMax ( llength[ 0 ], llength[ 1 ] );
      }
      if ( sppc->show_value_string () ) {
        lbl.text = sppc->value_string ();
      }
      ++col_total_idx;
    }
  }

  // Inputs area layout and widget creation
  ::Wdg::Sliders_Pad_Layout * sp_layout ( new Sliders_Pad_Layout ( _sp_data ) );
  sp_layout->setContentsMargins ( 0, 0, 0, 0 );
  sp_layout->set_header_widget ( header () );
  sp_layout->set_footer_widget ( footer () );

  for ( unsigned int gii = 0; gii < num_groups; ++gii ) {

    ::Wdg::Pad_Proxies_Group * sppg ( proxies_group ( gii ) );
    sppg->set_pad ( this );
    sppg->set_group_index ( gii );

    // Add widgets to the widgets groups
    for ( unsigned int cii = 0; cii < sppg->num_columns (); ++cii ) {
      ::Wdg::Pad_Proxies_Column * sppc ( sppg->column ( cii ) );

      // Slider widget
      if ( sppc->has_slider () ) {
        ::Wdg::Pad_Proxy_Slider * spps ( sppc->slider_proxy () );

        ::Wdg::DS_Slider * sl_wdg ( new ::Wdg::DS_Slider ( 0, _image_alloc ) );
        sl_wdg->set_maximum_index ( spps->slider_index_max () );
        sl_wdg->set_current_index ( spps->slider_index () );
        sl_wdg->setToolTip ( spps->tool_tip () );
        sl_wdg->setEnabled ( spps->is_enabled () );
        sl_wdg->set_wheel_degrees ( wheel_degrees () );
        sl_wdg->set_wdg_style_db ( wdg_style_db () );
        sl_wdg->set_style_id ( spps->style_id () );

        if ( spps->style () != 0 ) {
          ::Wdg::DS_Slider_Meta_Bg & meta_bg ( sl_wdg->meta_bg () );
          meta_bg.bg_show_image = spps->style ()->slider_has_minimum;
          meta_bg.bg_tick_min_idx = spps->style ()->slider_minimum_idx;
        }

        connect ( spps,
                  SIGNAL ( sig_enabled_changed ( bool ) ),
                  sl_wdg,
                  SLOT ( setEnabled ( bool ) ) );

        connect ( sl_wdg,
                  SIGNAL ( sig_current_index_changed ( unsigned long ) ),
                  spps,
                  SLOT ( set_slider_index ( unsigned long ) ) );

        connect ( spps,
                  SIGNAL ( sig_slider_index_changed ( unsigned long ) ),
                  sl_wdg,
                  SLOT ( set_current_index ( unsigned long ) ) );

        connect ( spps,
                  SIGNAL ( sig_slider_index_max_changed ( unsigned long ) ),
                  sl_wdg,
                  SLOT ( set_maximum_index ( unsigned long ) ) );

        spps->set_widget ( sl_wdg );
        _widgets.append ( sl_wdg );
        sp_layout->add_group_widget ( sl_wdg, gii, cii, 0 );
      }

      // Switch widget
      if ( sppc->has_switch () ) {
        ::Wdg::Pad_Proxy_Switch * spps ( sppc->switch_proxy () );

        ::Wdg::DS_Switch * sw_wdg ( new ::Wdg::DS_Switch ( 0, _image_alloc ) );

        sw_wdg->setChecked ( spps->switch_state () );
        sw_wdg->setToolTip ( spps->tool_tip () );
        sw_wdg->setEnabled ( spps->is_enabled () );
        sw_wdg->set_variant_id ( spps->variant_id () );
        sw_wdg->set_style_id ( spps->style_id () );

        connect ( spps,
                  SIGNAL ( sig_enabled_changed ( bool ) ),
                  sw_wdg,
                  SLOT ( setEnabled ( bool ) ) );

        connect ( sw_wdg,
                  SIGNAL ( toggled ( bool ) ),
                  spps,
                  SLOT ( set_switch_state ( bool ) ) );

        connect ( spps,
                  SIGNAL ( sig_switch_state_changed ( bool ) ),
                  sw_wdg,
                  SLOT ( setChecked ( bool ) ) );

        spps->set_widget ( sw_wdg );
        _widgets.append ( sw_wdg );
        sp_layout->add_group_widget ( sw_wdg, gii, cii, 1 );
      }
    }
  }

  setLayout ( sp_layout );
}

void
Sliders_Pad::set_proxies_groups (
    const QList<::Wdg::Pad_Proxies_Group * > & list_n )
{
  bool do_update ( false );
  if ( _proxies_groups.size () > 0 ) {
    clear_widgets ();
    do_update = true;
  }

  _proxies_groups = list_n;

  if ( _proxies_groups.size () > 0 ) {
    create_widgets ();
    do_update = true;
  }

  if ( do_update ) {
    _update_decoration = true;
    header_data ()->update_decoration = true;
    footer_data ()->update_decoration = true;
    update ();
  }
}

void
Sliders_Pad::clear_proxies_groups ()
{
  if ( _proxies_groups.size () > 0 ) {
    set_proxies_groups ( QList< Pad_Proxies_Group * > () );
  }
}

void
Sliders_Pad::update_colors ()
{
  {
    const QPalette & pal ( palette () );
    const QColor col_bg ( pal.color ( QPalette::Button ) );
    const QColor col_fg ( pal.color ( QPalette::ButtonText ) );
    QColor col = ::Wdg::col_mix ( col_bg, col_fg, 1, 1 );

    _sp_style->stem_pen.setColor ( col );
  }
}

bool
Sliders_Pad::set_focus_proxy ( unsigned int group_idx_n )
{
  return set_focus_proxy ( group_idx_n, 0, 0 );
}

bool
Sliders_Pad::set_focus_proxy ( unsigned int group_idx_n,
                               unsigned int column_idx_n,
                               unsigned int row_idx_n )
{
  bool success ( false );
  const unsigned int num_groups ( _proxies_groups.size () );
  if ( group_idx_n < num_groups ) {
    ::Wdg::Pad_Proxies_Group * sppg ( proxies_group ( group_idx_n ) );
    success = sppg->take_focus ( column_idx_n, row_idx_n );
  }
  return success;
}

void
Sliders_Pad::header_label_selected ( unsigned int group_idx_n,
                                     unsigned int column_idx_n )
{
  set_focus_proxy ( group_idx_n, column_idx_n, 0 );
}

void
Sliders_Pad::footer_label_selected ( unsigned int group_idx_n,
                                     unsigned int column_idx_n )
{
  if ( set_focus_proxy ( group_idx_n, column_idx_n, 0 ) ) {
    emit sig_footer_label_selected ( group_idx_n, column_idx_n );
  }
}

bool
Sliders_Pad::event ( QEvent * event_n )
{
  if ( event_n->type () == ::Wdg::evt_pass_event_focus ) {
    ::Wdg::Pass_Event_Focus * ev_fp (
        static_cast<::Wdg::Pass_Event_Focus * > ( event_n ) );

    _focus_info.clear ();
    if ( ev_fp->ev_focus.gotFocus () &&
         ( ev_fp->group_idx < num_proxies_groups () ) ) {
      _focus_info.has_focus = true;
      _focus_info.group_idx = ev_fp->group_idx;
      _focus_info.column_idx = ev_fp->column_idx;
      _focus_info.row_idx = ev_fp->row_idx;
    }

    { // Update header focus
      unsigned int group_idx ( ~0 );
      unsigned int col_idx ( ~0 );
      if ( _focus_info.has_focus ) {
        group_idx = _focus_info.group_idx;
        col_idx = _focus_info.column_idx;
      }
      header_cast ()->set_focus_idx ( group_idx, col_idx );
      footer_cast ()->set_focus_idx ( group_idx, col_idx );
    }

    emit sig_focus_changed ();
    update ();

    return true;
  }

  return QWidget::event ( event_n );
}

void
Sliders_Pad::resizeEvent ( QResizeEvent * event )
{
  //::std::cout << "Resize event " << width() << ":" << height() << "\n";
  QWidget::resizeEvent ( event );
  _update_decoration = true;
  header_data ()->update_decoration = true;
  footer_data ()->update_decoration = true;
}

void
Sliders_Pad::paintEvent ( QPaintEvent * event )
{
  QWidget::paintEvent ( event );

  if ( _update_decoration ) {
    _update_decoration = false;
    _sp_style->paint_base_decoration ();
  }

  {
    QPainter painter ( this );
    painter.setRenderHints ( QPainter::Antialiasing |
                             QPainter::TextAntialiasing |
                             QPainter::SmoothPixmapTransform );

    // Debug area painting
    // painter.setBrush ( Qt::yellow );
    // painter.setPen ( Qt::NoPen );
    // painter.drawRect ( rect() );

    for ( unsigned int gii = 0; gii < _sp_data->groups.size (); ++gii ) {
      painter.drawPicture ( 0, 0, _sp_data->groups[ gii ]->center_pic );
    }
  }
}

} // namespace Wdg
