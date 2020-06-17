/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_table_model.hpp"
#include "qsnd/mixer_hctl.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include <QFont>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Table_Model::Mixer_HCTL_Table_Model ( QObject * parent_n )
: QAbstractTableModel ( parent_n )
, _snd_mixer ( 0 )
, _iface_type_idx ( 0 )
, _num_columns ( 7 )
, _align_cc ( Qt::AlignHCenter | Qt::AlignVCenter )
, _align_lc ( Qt::AlignLeft | Qt::AlignVCenter )
{
  _str_dev_mask = tr ( "%1,%2" );

  _ttip_dev = _str_dev_mask;
  _ttip_dev = _ttip_dev.arg ( "Device" );
  _ttip_dev = _ttip_dev.arg ( "Subdevice" );

  _ttip_name_mask = "<div>%1</div>\n<div>(%2)</div>";

  _col_names[ 0 ] = tr ( "Name" );
  _col_ttips[ 0 ] = tr ( "Element name" );

  //: Idx - abbreviation for Index
  _col_names[ 1 ] = tr ( "Idx" );
  _col_ttips[ 1 ] = tr ( "Element index" );

  _col_names[ 2 ] = tr ( "Device" );
  _col_ttips[ 2 ] = tr ( "Device" );

  _col_names[ 3 ] = tr ( "Type" );
  _col_ttips[ 3 ] = tr ( "Element type" );

  //: Ch. - abbreviation for Channel
  _col_names[ 4 ] = tr ( "Ch." );
  _col_ttips[ 4 ] = tr ( "Channel count" );

  _col_names[ 5 ] = tr ( "Flags" );
  _col_ttips[ 5 ] = tr ( "Flags" );

  _col_names[ 6 ] = tr ( "Id" );
  _col_ttips[ 6 ] = tr ( "Numeric Id" );
}

Mixer_HCTL_Table_Model::~Mixer_HCTL_Table_Model ()
{
  set_snd_mixer ( 0 );
}

int
Mixer_HCTL_Table_Model::rowCount ( const QModelIndex & parent_n ) const
{
  int res ( 0 );
  if ( !parent_n.isValid () ) {
    res = _elems.size ();
  }
  return res;
}

int
Mixer_HCTL_Table_Model::columnCount ( const QModelIndex & parent_n ) const
{
  int res ( 0 );
  if ( !parent_n.isValid () ) {
    res = _num_columns;
  }
  return res;
}

QVariant
Mixer_HCTL_Table_Model::data ( const QModelIndex & index_n, int role_n ) const
{
  QVariant res;
  const int row ( index_n.row () );
  const int col ( index_n.column () );
  if ( !index_n.isValid () || index_n.parent ().isValid () || ( row < 0 ) ||
       ( row >= _elems.size () ) || ( col < 0 ) ||
       ( col >= (int)_num_columns ) ) {
    return res;
  }

  const ::QSnd::Mixer_HCTL_Elem * elem ( _elems[ row ] );
  const QString val ( "%1" );
  if ( col == 0 ) {

    // Element name
    if ( role_n == Qt::DisplayRole ) {
      QString valc ( elem->display_name () );
      res = valc;
    } else if ( role_n == Qt::ToolTipRole ) {
      QString valc ( elem->display_name () );
      if ( valc != elem->elem_name () ) {
        valc = _ttip_name_mask.arg ( valc );
        valc = valc.arg ( elem->elem_name () );
      }
      res = valc;
    }

  } else if ( col == 1 ) {

    // Element index
    if ( role_n == Qt::DisplayRole ) {
      res = val.arg ( elem->elem_index () );
    } else if ( role_n == Qt::TextAlignmentRole ) {
      res = QVariant ( _align_cc );
    }

  } else if ( col == 2 ) {

    // Element device
    if ( role_n == Qt::DisplayRole ) {
      QString valc ( _str_dev_mask );
      valc = valc.arg ( elem->device () );
      valc = valc.arg ( elem->subdevice () );
      res = valc;
    } else if ( role_n == Qt::ToolTipRole ) {
      res = _ttip_dev;
    } else if ( role_n == Qt::TextAlignmentRole ) {
      res = QVariant ( _align_cc );
    }

  } else if ( col == 3 ) {

    // Element type
    if ( role_n == Qt::DisplayRole ) {
      res = elem->elem_type_display_name ();
    } else if ( role_n == Qt::ToolTipRole ) {
      res = elem->elem_type_display_name ();
    }

  } else if ( col == 4 ) {

    // Element channel count
    if ( role_n == Qt::DisplayRole ) {
      res = val.arg ( elem->count () );
    } else if ( role_n == Qt::TextAlignmentRole ) {
      res = QVariant ( _align_cc );
    }

  } else if ( col == 5 ) {

    const ::QSnd::Mixer_HCTL_Info_Db * idb ( _snd_mixer->info_db () );

    // Element is readable
    if ( role_n == Qt::DisplayRole ) {
      QString txt;
      txt += idb->flag_readable_char ( elem->is_readable () );
      txt += idb->flag_writable_char ( elem->is_writable () );
      txt += idb->flag_active_char ( elem->is_active () );
      txt += idb->flag_volatile_char ( elem->is_volatile () );
      res = txt;
    } else if ( role_n == Qt::ToolTipRole ) {
      const QString wrap ( "<div>%1</div>" );
      QString txt_res;
      QString tmp;
      tmp = idb->flag_readable_text ( elem->is_readable () );
      txt_res += wrap.arg ( tmp );
      tmp = idb->flag_writable_text ( elem->is_writable () );
      txt_res += wrap.arg ( tmp );
      tmp = idb->flag_active_text ( elem->is_active () );
      txt_res += wrap.arg ( tmp );
      tmp = idb->flag_volatile_text ( elem->is_volatile () );
      txt_res += wrap.arg ( tmp );
      res = txt_res;
    } else if ( role_n == Qt::TextAlignmentRole ) {
      res = QVariant ( _align_cc );
    }

  } else if ( col == 6 ) {

    // Element numeric id
    if ( role_n == Qt::DisplayRole ) {
      res = val.arg ( elem->elem_numid () );
    } else if ( role_n == Qt::TextAlignmentRole ) {
      res = QVariant ( _align_cc );
    }
  }

  return res;
}

QVariant
Mixer_HCTL_Table_Model::headerData ( int section_n,
                                     Qt::Orientation orientation_n,
                                     int role_n ) const
{
  QVariant res;
  if ( ( orientation_n == Qt::Vertical ) || ( section_n < 0 ) ||
       ( section_n >= (int)_num_columns ) ) {
    return res;
  }

  if ( role_n == Qt::DisplayRole ) {
    res = _col_names[ section_n ];
  } else if ( role_n == Qt::ToolTipRole ) {
    res = _col_ttips[ section_n ];
  }

  return res;
}

::QSnd::Mixer_HCTL_Elem *
Mixer_HCTL_Table_Model::elem ( const QModelIndex & idx_n ) const
{
  ::QSnd::Mixer_HCTL_Elem * res ( 0 );

  if ( !idx_n.isValid () || ( idx_n.column () < 0 ) ||
       ( idx_n.column () >= columnCount () ) || ( idx_n.row () < 0 ) ||
       ( idx_n.column () >= rowCount () ) ) {
    return res;
  }

  if ( !idx_n.parent ().isValid () ) {
    if ( idx_n.row () < _elems.size () ) {
      res = _elems[ idx_n.row () ];
    }
  }

  return res;
}

void
Mixer_HCTL_Table_Model::set_snd_mixer ( ::QSnd::Mixer_HCTL * snd_mixer_n )
{
  if ( _snd_mixer != snd_mixer_n ) {
    clear ();

    _snd_mixer = snd_mixer_n;

    load ();
  }
}

void
Mixer_HCTL_Table_Model::set_iface_type_idx ( unsigned int type_n )
{
  if ( _iface_type_idx != type_n ) {
    beginResetModel ();
    clear ();

    _iface_type_idx = type_n;

    load ();
    endResetModel ();
  }
}

void
Mixer_HCTL_Table_Model::clear ()
{
  if ( _elems.size () > 0 ) {
    beginRemoveRows ( QModelIndex (), 0, _elems.size () - 1 );
    _elems.clear ();
    endRemoveRows ();
  }
}

void
Mixer_HCTL_Table_Model::load ()
{
  if ( snd_mixer () == 0 ) {
    return;
  }
  if ( snd_mixer ()->iface_type_count ( iface_type_idx () ) == 0 ) {
    return;
  }

  QList<::QSnd::Mixer_HCTL_Elem * > new_elems;

  //
  // Filter elements
  //
  const unsigned int num_elems ( snd_mixer ()->num_elems () );
  for ( unsigned int ii = 0; ii < num_elems; ++ii ) {

    ::QSnd::Mixer_HCTL_Elem * elem ( snd_mixer ()->elem ( ii ) );

    if ( elem->iface_type_idx () == iface_type_idx () ) {
      new_elems.append ( elem );
    }
  }

  if ( new_elems.size () > 0 ) {
    beginInsertRows ( QModelIndex (), 0, new_elems.size () - 1 );
    _elems = new_elems;
    endInsertRows ();
  }
}

} // namespace MWdg
