/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "switches_pad.hpp"
#include "wdg/color_methods.hpp"
#include "wdg/event_types.hpp"
#include "wdg/fill_columns_layout.hpp"
#include "wdg/pad_proxies_column.hpp"
#include "wdg/pad_proxies_group.hpp"
#include "wdg/pad_proxy_enum.hpp"
#include "wdg/pad_proxy_switch.hpp"
#include "wdg/pass_events.hpp"
#include "wdg/switches_pad_widgets_group.hpp"
#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QVBoxLayout>
#include <iostream>

namespace Wdg
{

Switches_Pad::Switches_Pad ( QWidget * parent_n )
: QWidget ( parent_n )
{
  _stem_pen.setWidth ( 1 );
  update_colors ();
}

Switches_Pad::~Switches_Pad ()
{
  clear_proxies_groups ();
}

void
Switches_Pad::set_viewport_geometry ( const QRect & rect_n )
{
  //::std::cout << "Switches_Pad::set_viewport_geometry\n";
  if ( _viewport != rect_n ) {
    _viewport = rect_n;

    if ( layout () != 0 ) {
      ::Wdg::Fill_Columns_Layout * lay_cols (
          static_cast<::Wdg::Fill_Columns_Layout * > ( layout () ) );
      lay_cols->set_viewport_geometry ( _viewport );
    }
  }
}

void
Switches_Pad::set_proxies_groups (
    const QList<::Wdg::Pad_Proxies_Group * > & groups_n )
{
  if ( _proxies_groups.size () > 0 ) {
    clear_widgets_groups ();
  }

  _proxies_groups = groups_n;

  if ( _proxies_groups.size () > 0 ) {
    create_widgets_groups ();
  }
}

void
Switches_Pad::clear_proxies_groups ()
{
  if ( _proxies_groups.size () > 0 ) {
    set_proxies_groups ( QList<::Wdg::Pad_Proxies_Group * > () );
  }
}

void
Switches_Pad::clear_widgets_groups ()
{
  if ( layout () != 0 ) {
    delete layout ();
  }

  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    ::Wdg::Pad_Proxies_Group * grp ( _proxies_groups[ ii ] );
    grp->set_pad ( 0 );
    for ( unsigned int jj = 0; jj < grp->num_columns (); ++jj ) {
      ::Wdg::Pad_Proxies_Column * col ( grp->column ( jj ) );
      if ( col->switch_proxy () != 0 ) {
        col->switch_proxy ()->set_widget ( 0 );
      }
      if ( col->enum_proxy () != 0 ) {
        col->enum_proxy ()->set_widget ( 0 );
      }
    }
  }

  if ( _widgets_groups.size () > 0 ) {
    for ( int ii = 0; ii < _widgets_groups.size (); ++ii ) {
      delete _widgets_groups[ ii ];
    }
    _widgets_groups.clear ();
  }
}

void
Switches_Pad::create_widgets_groups ()
{
  //::std::cout << "Mixer_Switches::create_widgets_groups\n";

  if ( _proxies_groups.size () == 0 ) {
    return;
  }

  ::Wdg::Fill_Columns_Layout * lay_cols ( new ::Wdg::Fill_Columns_Layout );
  lay_cols->setContentsMargins ( 0, 0, 0, 0 );
  lay_cols->set_viewport_geometry ( _viewport );

  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    ::Wdg::Pad_Proxies_Group * sppg ( _proxies_groups[ ii ] );
    sppg->set_pad ( this );
    sppg->set_group_index ( ii );

    QScopedPointer<::Wdg::Switches_Pad_Widgets_Group > spwg (
        new ::Wdg::Switches_Pad_Widgets_Group );

    if ( sppg->num_columns () > 1 ) {
      QLabel * label ( new QLabel ( spwg.data () ) );
      label->setText ( sppg->group_name () );
      label->setToolTip ( sppg->tool_tip () );
      label->setFocusPolicy ( Qt::ClickFocus );
      label->setContextMenuPolicy ( Qt::NoContextMenu );

      {
        QFont fnt ( font () );
        fnt.setBold ( true );
        label->setFont ( fnt );
      }

      label->installEventFilter ( spwg.data () );

      spwg->set_label ( label );
      spwg->set_stem_pen ( _stem_pen );
    }

    for ( unsigned int jj = 0; jj < sppg->num_columns (); ++jj ) {

      ::Wdg::Pad_Proxies_Column * sppc ( sppg->column ( jj ) );

      if ( sppc->has_enum () ) {

        // Is enumerated
        ::Wdg::Pad_Proxy_Enum * spp_enum ( sppc->enum_proxy () );
        ::Wdg::Switches_Pad_Widgets * spw ( new Switches_Pad_Widgets );

        { // Label
          QLabel * label ( new QLabel ( spwg.data () ) );
          label->setText ( spp_enum->item_name () );
          label->setToolTip ( spp_enum->tool_tip () );
          spw->set_label_wdg ( label );
        }

        { // Combo box
          // Setup selectable items
          QComboBox * cbox ( new QComboBox ( spwg.data () ) );
          for ( int ee = 0; ee < spp_enum->enum_num_items (); ++ee ) {
            cbox->addItem ( spp_enum->enum_item_name ( ee ) );
          }
          cbox->setCurrentIndex ( spp_enum->enum_index () );
          cbox->setToolTip ( spp_enum->tool_tip () );

          connect ( cbox,
                    SIGNAL ( currentIndexChanged ( int ) ),
                    spp_enum,
                    SLOT ( set_enum_index ( int ) ) );
          connect ( spp_enum,
                    SIGNAL ( sig_enum_index_changed ( int ) ),
                    cbox,
                    SLOT ( setCurrentIndex ( int ) ) );

          spw->set_input_wdg ( cbox );
          spp_enum->set_widget ( cbox );
        }

        spwg->append_widgets ( spw );
        continue;
      }

      if ( sppc->has_switch () ) {

        // Is switch
        ::Wdg::Pad_Proxy_Switch * spp_switch ( sppc->switch_proxy () );
        Switches_Pad_Widgets * spw ( new Switches_Pad_Widgets );

        QCheckBox * switch_wdg ( new QCheckBox ( spwg.data () ) );

        if ( sppg->num_columns () > 1 ) {
          switch_wdg->setText ( spp_switch->item_name () );
        } else {
          switch_wdg->setText ( sppg->group_name () );
        }
        switch_wdg->setChecked ( spp_switch->switch_state () );
        switch_wdg->setToolTip ( spp_switch->tool_tip () );
        switch_wdg->setPalette ( palette () );

        connect ( switch_wdg,
                  SIGNAL ( toggled ( bool ) ),
                  spp_switch,
                  SLOT ( set_switch_state ( bool ) ) );

        connect ( spp_switch,
                  SIGNAL ( sig_switch_state_changed ( bool ) ),
                  switch_wdg,
                  SLOT ( setChecked ( bool ) ) );

        spw->set_input_wdg ( switch_wdg );
        spp_switch->set_widget ( switch_wdg );

        spwg->append_widgets ( spw );
        continue;
      }
    }

    // Append widgets group to the list
    if ( spwg->num_widgets () > 0 ) {
      _widgets_groups.append ( spwg.take () );
    }
  }

  // Append to layout
  for ( int ii = 0; ii < _widgets_groups.size (); ++ii ) {
    lay_cols->addWidget ( _widgets_groups[ ii ] );
  }
  setLayout ( lay_cols );
}

void
Switches_Pad::set_focus_proxy ( unsigned int proxies_group_idx_n )
{
  set_focus_proxy ( proxies_group_idx_n, 0 );
}

void
Switches_Pad::set_focus_proxy ( unsigned int proxies_group_idx_n,
                                unsigned int proxy_idx_n )
{
  const unsigned int num_groups ( _widgets_groups.size () );
  if ( proxies_group_idx_n < num_groups ) {
    ::Wdg::Switches_Pad_Widgets_Group * spwg (
        _widgets_groups[ proxies_group_idx_n ] );

    QWidget * wdg ( 0 );

    // Select the slider of given widget set
    if ( proxy_idx_n < spwg->num_widgets () ) {
      wdg = spwg->widgets ( proxy_idx_n )->input_wdg ();
    }

    // No widget found so far - Take first slider then
    if ( wdg == 0 ) {
      for ( unsigned int ii = 0; ii < spwg->num_widgets (); ++ii ) {
        ::Wdg::Switches_Pad_Widgets * spw ( spwg->widgets ( ii ) );
        if ( spw->input_wdg () != 0 ) {
          wdg = spw->input_wdg ();
          break;
        }
      }
    }

    if ( wdg != 0 ) {
      wdg->setFocus ();
    }
  }
}

void
Switches_Pad::update_colors ()
{
  {
    QPalette pal ( palette () );
    const QColor col_bg ( pal.color ( QPalette::Button ) );
    const QColor col_fg ( pal.color ( QPalette::ButtonText ) );
    QColor col = ::Wdg::col_mix ( col_bg, col_fg, 1, 1 );

    _stem_pen.setColor ( col );
  }
}

bool
Switches_Pad::event ( QEvent * event_n )
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
    }

    emit sig_focus_changed ();

    return true;
  }

  return QWidget::event ( event_n );
}

} // namespace Wdg
