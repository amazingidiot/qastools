/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl.hpp"
#include "mwdg/mixer_hctl_edit_bool.hpp"
#include "mwdg/mixer_hctl_edit_enum.hpp"
#include "mwdg/mixer_hctl_edit_int.hpp"
#include "mwdg/mixer_hctl_edit_unsupported.hpp"
#include "mwdg/mixer_hctl_editor_data.hpp"
#include "mwdg/mixer_hctl_proxy.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_HCTL::Mixer_HCTL ( QWidget * parent_n )
: QWidget ( parent_n )
, _editor_pad ( 0 )
{
  _editor_data.reset ( new ::MWdg::Mixer_HCTL_Editor_Data );

  {
    const QString dmask ( "<div>%1</div>\n<div>(%2)</div>" );
    _ttip_name_lbl_mask = dmask.arg ( tr ( "Element name" ) );
  }

  // Localized shared strings
  _editor_data->str_joined = tr ( "Joined" );
  _editor_data->ttip_grid_lbl_elem = tr ( "Element index" );
  _editor_data->ttip_grid_lbl_channel = tr ( "Channel %1" );

  _editor_data->str_list_channel = tr ( "Ch. %1" );
  _editor_data->ttip_list_channel = tr ( "Channel %1" );
  {
    const QString dmask ( "<div>%1</div>" );
    QString txt[ 2 ];
    txt[ 0 ] = tr ( "Index: %1" );
    txt[ 1 ] = tr ( "Channel: %2" );
    _editor_data->ttip_grid_widget += dmask.arg ( txt[ 0 ] );
    _editor_data->ttip_grid_widget += "\n";
    _editor_data->ttip_grid_widget += dmask.arg ( txt[ 1 ] );
  }

  // Pad widget layout
  {
    QVBoxLayout * lay_pad_wdg ( new QVBoxLayout );
    lay_pad_wdg->setContentsMargins ( 0, 0, 0, 0 );
    _pad_wdg.setLayout ( lay_pad_wdg );
  }

  // Info widget 1 layout
  {
    QHBoxLayout * lay_elem_name ( new QHBoxLayout );
    lay_elem_name->setContentsMargins ( 0, 0, 0, 0 );
    lay_elem_name->addWidget ( &_info_lbl_name );
    lay_elem_name->addStretch ();

    QHBoxLayout * lay_info_items_1 ( new QHBoxLayout );
    lay_info_items_1->setContentsMargins ( 0, 0, 0, 0 );
    {
      QGridLayout * lay_items ( new QGridLayout );
      lay_items->setContentsMargins ( 0, 0, 0, 0 );

      unsigned int row ( 0 );
      unsigned int col ( 0 );
      lay_items->addWidget ( &_info_lbl_count.name, row, col++ );
      lay_items->addWidget ( &_info_lbl_count.value, row, col++ );
      lay_items->addWidget ( &_info_lbl_flags.name, row, col++ );
      lay_items->addWidget ( &_info_lbl_flags.value, row, col++ );

      ++row;
      col = 0;
      lay_items->addWidget ( &_info_lbl_index.name, row, col++ );
      lay_items->addWidget ( &_info_lbl_index.value, row, col++ );
      lay_items->addWidget ( &_info_lbl_dev.name, row, col++ );
      lay_items->addWidget ( &_info_lbl_dev.value, row, col++ );
      lay_items->addWidget ( &_info_lbl_numid.name, row, col++ );
      lay_items->addWidget ( &_info_lbl_numid.value, row, col++ );

      lay_info_items_1->addLayout ( lay_items );
      lay_info_items_1->addStretch ();
    }

    QVBoxLayout * lay_info_wdg ( new QVBoxLayout );
    lay_info_wdg->setContentsMargins ( 0, 0, 0, 0 );
    lay_info_wdg->addLayout ( lay_elem_name );
    lay_info_wdg->addLayout ( lay_info_items_1 );

    _info_wdg.setLayout ( lay_info_wdg );
  }

  // Item name labels
  _info_lbl_index.name.setText ( tr ( "Index:" ) );
  _info_lbl_dev.name.setText ( tr ( "Device:" ) );
  _info_lbl_flags.name.setText ( tr ( "Flags:" ) );
  _info_lbl_count.name.setText ( tr ( "Channels:" ) );
  _info_lbl_numid.name.setText ( tr ( "Num. id:" ) );

  // Item name tooltips
  _info_lbl_index.name.setToolTip ( tr ( "Element index" ) );
  _info_lbl_dev.name.setToolTip ( tr ( "Device" ) );
  _info_lbl_flags.name.setToolTip ( tr ( "Flags" ) );
  _info_lbl_count.name.setToolTip ( tr ( "Channel count" ) );
  _info_lbl_numid.name.setToolTip ( tr ( "Numeric Id" ) );

  // Item values
  _info_dev_mask = "%1,%2";
  _info_lbl_index.value.set_min_text ( "1000" );
  _info_lbl_dev.value.set_min_text ( "99,99" );
  _info_lbl_flags.value.set_min_text ( "MMMM" );
  _info_lbl_count.value.set_min_text ( "1000" );
  _info_lbl_numid.value.set_min_text ( "1000" );

  // Item value tooltips
  {
    QString ttip_dev ( _info_dev_mask );
    ttip_dev = ttip_dev.arg ( tr ( "Device" ) );
    ttip_dev = ttip_dev.arg ( tr ( "Subdevice" ) );

    _info_lbl_index.value.setToolTip ( _info_lbl_index.name.toolTip () );
    _info_lbl_dev.value.setToolTip ( ttip_dev );

    _info_lbl_count.value.setToolTip ( _info_lbl_count.name.toolTip () );
    _info_lbl_numid.value.setToolTip ( _info_lbl_numid.name.toolTip () );
  }

  // Item alignment
  {
    const Qt::Alignment align_cc ( Qt::AlignHCenter | Qt::AlignVCenter );
    // const Qt::Alignment align_lc ( Qt::AlignLeft | Qt::AlignVCenter );
    _info_lbl_index.value.setAlignment ( align_cc );
    _info_lbl_dev.value.setAlignment ( align_cc );
    _info_lbl_flags.value.setAlignment ( align_cc );
    _info_lbl_count.value.setAlignment ( align_cc );
    _info_lbl_numid.value.setAlignment ( align_cc );
  }

  // Set final layout
  {
    QVBoxLayout * lay_vbox ( new QVBoxLayout );
    lay_vbox->addWidget ( &_info_wdg, 0 );
    lay_vbox->addWidget ( &_pad_wdg, 1 );
    setLayout ( lay_vbox );
  }

  update_info ();
}

Mixer_HCTL::~Mixer_HCTL ()
{
  clear ();
}

::QSnd::Mixer_HCTL_Elem_Group *
Mixer_HCTL::snd_elem_group () const
{
  return _editor_data->snd_elem_group;
}

unsigned int
Mixer_HCTL::elem_idx () const
{
  return _editor_data->elem_idx;
}

const ::QSnd::Mixer_HCTL_Info_Db *
Mixer_HCTL::ctl_info_db () const
{
  return _editor_data->ctl_info_db;
}

void
Mixer_HCTL::set_ctl_info_db ( const ::QSnd::Mixer_HCTL_Info_Db * info_db_n )
{
  _editor_data->ctl_info_db = info_db_n;
}

void
Mixer_HCTL::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  if ( _editor_pad != 0 ) {
    _editor_pad->set_inputs_setup ( 0 );
  }

  _inputs_setup = setup_n;

  if ( inputs_setup () != 0 ) {
    if ( _editor_pad != 0 ) {
      _editor_pad->set_inputs_setup ( inputs_setup () );
    }
  }
}

const ::Wdg::DS_Widget_Style_Db *
Mixer_HCTL::wdg_style_db () const
{
  return _editor_data->wdg_style_db;
}

void
Mixer_HCTL::set_wdg_style_db ( const ::Wdg::DS_Widget_Style_Db * style_db_n )
{
  _editor_data->wdg_style_db = style_db_n;
}

::dpe::Image_Allocator *
Mixer_HCTL::image_alloc () const
{
  return _editor_data->image_alloc;
}

void
Mixer_HCTL::set_image_alloc ( ::dpe::Image_Allocator * alloc_n )
{
  _editor_data->image_alloc = alloc_n;
}

void
Mixer_HCTL::clear ()
{
  _editor_pad.reset ();
  _editor_data->snd_elem_group = 0;
}

void
Mixer_HCTL::set_snd_elem_group ( ::QSnd::Mixer_HCTL_Elem_Group * elem_group_n,
                                 unsigned int index_n )
{
  if ( ( snd_elem_group () == elem_group_n ) && ( elem_idx () == index_n ) ) {
    return;
  }

  clear ();

  _editor_data->snd_elem_group = elem_group_n;
  _editor_data->elem_idx = index_n;

  if ( snd_elem_group () != 0 ) {
    setup_widgets ();
  }

  update_info ();
}

void
Mixer_HCTL::setup_widgets ()
{
  if ( snd_elem_group () == 0 ) {
    return;
  }
  if ( snd_elem_group ()->num_elems () == 0 ) {
    return;
  }

  ::QSnd::Mixer_HCTL_Elem * elem0 ( snd_elem_group ()->elem ( 0 ) );
  if ( elem0->is_boolean () ) {
    _editor_pad.reset (
        new Mixer_HCTL_Edit_Bool ( _editor_data.data (), this ) );
  } else if ( elem0->is_enumerated () ) {
    _editor_pad.reset (
        new Mixer_HCTL_Edit_Enum ( _editor_data.data (), this ) );
  } else if ( elem0->is_integer () ) {
    _editor_pad.reset (
        new Mixer_HCTL_Edit_Int ( _editor_data.data (), this ) );
  } else {
    _editor_pad.reset (
        new Mixer_HCTL_Edit_Unsupported ( _editor_data.data (), this ) );
  }

  if ( _editor_pad != 0 ) {
    _pad_wdg.layout ()->addWidget ( _editor_pad.data () );
    _editor_pad->set_inputs_setup ( inputs_setup () );
  }
}

void
Mixer_HCTL::update_info ()
{
  QFont name_fnt ( font () );
  QString name_val;
  QString name_ttip;

  _info_wdg.setEnabled ( ( snd_elem_group () != 0 ) );

  // Clear
  {
    QString clear_txt ( "" );
    _info_lbl_name.setText ( clear_txt );
    _info_lbl_index.value.setText ( clear_txt );
    _info_lbl_dev.value.setText ( clear_txt );
    _info_lbl_flags.value.setText ( clear_txt );
    _info_lbl_count.value.setText ( clear_txt );
    _info_lbl_numid.value.setText ( clear_txt );
  }

  if ( snd_elem_group () == 0 ) {

    name_fnt.setItalic ( true );
    name_val = tr ( "No element selected" );

  } else {

    const ::QSnd::Mixer_HCTL_Elem * elem_first (
        snd_elem_group ()->elem ( 0 ) );

    const QString dname ( elem_first->display_name () );

    name_val = "<h3>%1</h3>";
    name_val = name_val.arg ( dname );

    // Tooltip
    name_ttip = _ttip_name_lbl_mask.arg ( elem_first->elem_name () );

    if ( elem_idx () < snd_elem_group ()->num_elems () ) {

      const QString val ( "%1" );
      QString valc;

      ::QSnd::Mixer_HCTL_Elem * elem (
          snd_elem_group ()->elem ( elem_idx () ) );

      valc = val.arg ( elem->elem_index () );
      _info_lbl_index.value.setText ( valc );

      valc = _info_dev_mask;
      valc = valc.arg ( elem->device () );
      valc = valc.arg ( elem->subdevice () );
      _info_lbl_dev.value.setText ( valc );

      if ( ctl_info_db () != 0 ) {
        const QString wrap ( "<div>%1</div>" );
        QString txt;
        QString accu;

        accu = ctl_info_db ()->flag_readable_char ( elem->is_readable () );
        accu += ctl_info_db ()->flag_writable_char ( elem->is_writable () );
        accu += ctl_info_db ()->flag_active_char ( elem->is_active () );
        accu += ctl_info_db ()->flag_volatile_char ( elem->is_volatile () );
        _info_lbl_flags.value.setText ( accu );

        txt = ctl_info_db ()->flag_readable_text ( elem->is_readable () );
        accu = wrap.arg ( txt );
        txt = ctl_info_db ()->flag_writable_text ( elem->is_writable () );
        accu += wrap.arg ( txt );
        txt = ctl_info_db ()->flag_active_text ( elem->is_active () );
        accu += wrap.arg ( txt );
        txt = ctl_info_db ()->flag_volatile_text ( elem->is_volatile () );
        accu += wrap.arg ( txt );
        _info_lbl_flags.value.setToolTip ( accu );
      }

      valc = val.arg ( elem->count () );
      _info_lbl_count.value.setText ( valc );

      valc = val.arg ( elem->elem_numid () );
      _info_lbl_numid.value.setText ( valc );
    }
  }

  _info_lbl_name.setText ( name_val );
  _info_lbl_name.setToolTip ( name_ttip );
  _info_lbl_name.setFont ( name_fnt );
}

} // namespace MWdg
