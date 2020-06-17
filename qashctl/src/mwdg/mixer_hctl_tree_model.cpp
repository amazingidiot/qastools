/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_tree_model.hpp"
#include "qsnd/mixer_hctl.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include <QFont>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Tree_Model::Mixer_HCTL_Tree_Model ( QObject * parent_n )
: QStandardItemModel ( parent_n )
, _snd_mixer ( 0 )
{
}

Mixer_HCTL_Tree_Model::~Mixer_HCTL_Tree_Model ()
{
  set_snd_mixer ( 0 );
}

void
Mixer_HCTL_Tree_Model::set_snd_dir_foreground ( unsigned int dir_n,
                                                const QBrush & brush_n )
{
  _brush_snd_dir[ dir_n ] = brush_n;
}

unsigned int
Mixer_HCTL_Tree_Model::index_data ( const QModelIndex & idx_n,
                                    ::QSnd::Mixer_HCTL_Elem_Group ** grp_n,
                                    unsigned int * elem_index_n,
                                    unsigned int * iface_type_n ) const
{
  unsigned int res ( 0 );

  if ( grp_n == 0 ) {
    return res;
  }

  if ( !idx_n.isValid () || ( idx_n.column () != 0 ) || ( idx_n.row () < 0 ) ) {
    return res;
  }

  const QModelIndex idx_base ( invisibleRootItem ()->index () );

  if ( idx_n == idx_base ) {
    return res;
  }

  const unsigned int invalid_val ( ~0 );
  unsigned int type_grp ( _type_groups.size () );
  unsigned int type_row ( invalid_val );
  unsigned int elem_idx ( invalid_val );

  {
    const QModelIndex idx_p0 ( idx_n.parent () );

    if ( idx_p0 == idx_base ) {
      res = 1;
      type_grp = idx_n.row ();
    } else {
      const QModelIndex idx_p1 ( idx_p0.parent () );
      if ( idx_p1 == idx_base ) {
        res = 2;
        type_grp = idx_p0.row ();
        type_row = idx_n.row ();
      } else {
        const QModelIndex idx_p2 ( idx_p1.parent () );
        if ( idx_p2 == idx_base ) {
          res = 3;
          type_grp = idx_p1.row ();
          type_row = idx_p0.row ();
          elem_idx = idx_n.row ();
        }
      }
    }
  }

  const Type_Group * tgrp ( 0 );
  ::QSnd::Mixer_HCTL_Elem_Group * grp ( 0 );

  if ( res >= 1 ) {
    if ( (int)type_grp < _type_groups.size () ) {
      tgrp = _type_groups[ type_grp ];
    } else {
      res = 0;
    }
  }

  if ( res >= 2 ) {
    if ( (int)type_row < tgrp->list.size () ) {
      grp = tgrp->list[ type_row ];
    } else {
      res = 0;
    }
  }

  if ( res == 3 ) {
    if ( elem_idx >= grp->num_elems () ) {
      res = 0;
    }
  }

  if ( res == 2 ) {
    elem_idx = grp->num_elems ();
    if ( elem_idx == 1 ) {
      --elem_idx;
    }
  }

  if ( ( grp_n != 0 ) && ( grp != 0 ) ) {
    *grp_n = grp;
  }

  if ( ( iface_type_n != 0 ) && ( tgrp != 0 ) ) {
    *iface_type_n = tgrp->iface_type_idx;
  }

  if ( elem_index_n != 0 ) {
    *elem_index_n = elem_idx;
  }

  return res;
}

QModelIndex
Mixer_HCTL_Tree_Model::elem_index ( ::QSnd::Mixer_HCTL_Elem * elem_n ) const
{
  QModelIndex res;

  if ( ( elem_n != 0 ) && ( _type_groups.size () > 0 ) ) {
    bool found ( false );
    unsigned int tidx ( 0 );
    unsigned int gidx ( 0 );
    unsigned int eidx ( 0 );
    ::QSnd::Mixer_HCTL_Elem_Group * grp ( 0 );

    const unsigned int num_types ( _type_groups.size () );
    for ( unsigned int tgii = 0; tgii < num_types; ++tgii ) {
      const Type_Group * tgrp ( _type_groups[ tgii ] );

      const unsigned int num_grps ( tgrp->list.size () );
      for ( unsigned int gii = 0; gii < num_grps; ++gii ) {
        grp = tgrp->list[ gii ];
        for ( unsigned int eii = 0; eii < grp->num_elems (); ++eii ) {
          if ( grp->elem ( eii ) == elem_n ) {
            tidx = tgii;
            gidx = gii;
            eidx = eii;
            found = true;
            goto loop_break;
          }
        }
      }
    }

  loop_break:

    if ( found ) {
      res = index ( tidx, 0, invisibleRootItem ()->index () );
      res = index ( gidx, 0, res );
      if ( grp->num_elems () > 1 ) {
        res = index ( eidx, 0, res );
      }
    }
  }

  return res;
}

QModelIndex
Mixer_HCTL_Tree_Model::elem_desc_index ( const QString & iface_name_n,
                                         const QString & elem_name_n,
                                         unsigned int elem_idx_n ) const
{
  QModelIndex res;
  if ( iface_name_n.isEmpty () || ( _snd_mixer == 0 ) ) {
    return res;
  }

  const ::QSnd::Mixer_HCTL_Info_Db * idb ( _snd_mixer->info_db () );

  Type_Group * tgrp ( 0 );
  for ( int ii = 0; ii < _type_groups.size (); ++ii ) {
    Type_Group * tgrp_cur ( _type_groups[ ii ] );
    if ( iface_name_n == idb->iface_name ( tgrp_cur->iface_type_idx ) ) {
      tgrp = tgrp_cur;
      res = index ( ii, 0 );
      break;
    }
  }

  if ( ( tgrp == 0 ) || ( elem_name_n.isEmpty () ) ) {
    return res;
  }

  const ::QSnd::Mixer_HCTL_Elem_Group * grp ( 0 );
  for ( int ii = 0; ii < tgrp->list.size (); ++ii ) {
    const ::QSnd::Mixer_HCTL_Elem_Group * grp_cur ( tgrp->list[ ii ] );
    if ( elem_name_n == grp_cur->elem ( 0 )->elem_name () ) {
      grp = grp_cur;
      res = index ( ii, 0, res );
      break;
    }
  }

  if ( grp != 0 ) {
    if ( ( grp->num_elems () > 1 ) && ( elem_idx_n < grp->num_elems () ) ) {
      res = index ( elem_idx_n, 0, res );
    }
  }

  return res;
}

void
Mixer_HCTL_Tree_Model::set_snd_mixer ( ::QSnd::Mixer_HCTL * snd_mixer_n )
{
  if ( _snd_mixer != snd_mixer_n ) {
    clear ();

    _snd_mixer = snd_mixer_n;

    load ();
  }
}

void
Mixer_HCTL_Tree_Model::clear ()
{
  invisibleRootItem ()->removeRows ( 0, invisibleRootItem ()->rowCount () );

  for ( int ii = 0; ii < _type_groups.size (); ++ii ) {
    Type_Group * tgrp ( _type_groups[ ii ] );
    for ( int jj = 0; jj < tgrp->list.size (); ++jj ) {
      delete tgrp->list[ jj ];
    }
    delete tgrp;
  }
  _type_groups.clear ();
}

void
Mixer_HCTL_Tree_Model::load ()
{
  if ( snd_mixer () == 0 ) {
    return;
  }

  const unsigned int num_types ( snd_mixer ()->iface_types_avail () );
  const unsigned int num_elems ( snd_mixer ()->num_elems () );

  if ( num_types == 0 ) {
    return;
  }

  for ( unsigned int tii = 0; tii < num_types; ++tii ) {
    Type_Group * tgrp ( new Type_Group );
    tgrp->iface_type_idx = snd_mixer ()->iface_avail_type ( tii );
    _type_groups.append ( tgrp );
  }

  // Find groups for each interface type
  for ( unsigned int ii = 0; ii < num_elems; ++ii ) {

    ::QSnd::Mixer_HCTL_Elem * elem ( snd_mixer ()->elem ( ii ) );
    Type_Group * tgrp ( 0 );

    for ( unsigned int tii = 0; tii < num_types; ++tii ) {
      tgrp = _type_groups[ tii ];
      if ( elem->iface_type_idx () == tgrp->iface_type_idx ) {
        break;
      }
      tgrp = 0;
    }

    if ( tgrp != 0 ) {
      ::QSnd::Mixer_HCTL_Elem_Group * grp ( new ::QSnd::Mixer_HCTL_Elem_Group );
      grp->append_elem ( elem );
      tgrp->list.append ( grp );
    }
  }

  // Moves all items with the same name into a single group
  for ( unsigned int tii = 0; tii < num_types; ++tii ) {
    compress_group_list ( _type_groups[ tii ]->list );
  }

  //
  // Create model items
  //
  const QString val ( "%1" );
  const QString ttip_suff ( "(%1)" );
  const QString num_suff ( " [%1]" );
  const QString idx_suff ( ", %1" );
  const QString dmask ( "<div>%1</div>" );

  QFont fnt_default;

  for ( unsigned int tii = 0; tii < num_types; ++tii ) {
    const Type_Group * tgrp ( _type_groups[ tii ] );

    QStandardItem * type_item ( new QStandardItem );
    type_item->setSelectable ( true );
    type_item->setEditable ( false );
    type_item->setText ( snd_mixer ()->info_db ()->iface_display_name (
        snd_mixer ()->iface_avail_type ( tii ) ) );
    {
      QFont fnt ( fnt_default );
      fnt.setBold ( true );
      type_item->setFont ( fnt );
    }

    for ( int gii = 0; gii < tgrp->list.size (); ++gii ) {

      const ::QSnd::Mixer_HCTL_Elem_Group * grp ( tgrp->list[ gii ] );
      const ::QSnd::Mixer_HCTL_Elem * elem_first ( grp->elem ( 0 ) );

      QStandardItem * grp_item ( new QStandardItem );
      grp_item->setSelectable ( true );
      grp_item->setEditable ( false );

      QString elm_name ( elem_first->elem_name () );
      QString elm_disp_name ( elem_first->display_name () );
      QString it_text ( elm_disp_name );
      QString it_ttip ( dmask.arg ( elm_disp_name ) );
      QFont fnt ( fnt_default );
      QBrush fg_brush ( get_foreground ( elm_name ) );

      if ( elm_name != elm_disp_name ) {
        // Append untranslated real name to tooltip
        it_ttip.append ( "\n" );
        it_ttip.append ( dmask.arg ( ttip_suff.arg ( elm_name ) ) );
      }

      const unsigned int num_grp_elms ( grp->num_elems () );
      if ( num_grp_elms > 1 ) {
        fnt.setItalic ( true );
        for ( unsigned int jj = 0; jj < num_grp_elms; ++jj ) {
          const ::QSnd::Mixer_HCTL_Elem * elem ( grp->elem ( jj ) );
          QStandardItem * elm_item ( new QStandardItem );
          {
            QString txt ( elm_disp_name );
            txt += idx_suff.arg ( elem->elem_index () );
            elm_item->setText ( txt );
          }
          elm_item->setToolTip ( it_ttip );
          elm_item->setSelectable ( true );
          elm_item->setEditable ( false );
          elm_item->setForeground ( fg_brush );
          grp_item->appendRow ( elm_item );
        }

        elm_disp_name += num_suff.arg ( num_grp_elms );
      }

      grp_item->setFont ( fnt );
      grp_item->setText ( elm_disp_name );
      grp_item->setToolTip ( it_ttip );
      grp_item->setForeground ( fg_brush );

      type_item->appendRow ( grp_item );
    }

    invisibleRootItem ()->appendRow ( type_item );
  }
}

void
Mixer_HCTL_Tree_Model::compress_group_list (
    QList<::QSnd::Mixer_HCTL_Elem_Group * > & lst_n )
{
  QString span_name;
  ::QSnd::Mixer_HCTL_Elem_Group * span_grp ( 0 );
  ::QSnd::Mixer_HCTL_Elem * span_elem ( 0 );
  unsigned int span_start ( 0 );
  unsigned int span ( 0 );

  for ( int gii = 0; gii < lst_n.size (); ++gii ) {
    ::QSnd::Mixer_HCTL_Elem_Group * grp ( lst_n[ gii ] );
    ::QSnd::Mixer_HCTL_Elem * elem ( grp->elem ( 0 ) );
    bool match ( false );
    if ( span_grp != 0 ) {
      match = ( elem->elem_name () == span_name ) &&
              ( elem->elem_type () == span_elem->elem_type () ) &&
              ( elem->iface () == span_elem->iface () ) &&
              ( elem->elem_index () != span_elem->elem_index () );
    }
    if ( match ) {
      ++span;
    }

    if ( !match || ( gii == ( lst_n.size () - 1 ) ) ) {
      if ( span > 0 ) {
        for ( unsigned int jj = 0; jj < span; ++jj ) {
          ::QSnd::Mixer_HCTL_Elem_Group * grp_cur (
              lst_n[ span_start + 1 + jj ] );
          span_grp->append_elem ( grp_cur->elem ( 0 ) );
          grp_cur->clear ();
        }
      }

      span_name = elem->elem_name ();
      span_grp = grp;
      span_elem = elem;
      span_start = gii;
      span = 0;
    }
  }

  // Remove empty groups

  int gii ( 0 );
  while ( gii < lst_n.size () ) {
    ::QSnd::Mixer_HCTL_Elem_Group * grp ( lst_n[ gii ] );
    if ( grp->num_elems () == 0 ) {
      lst_n.removeAt ( gii );
      delete grp;
    } else {
      ++gii;
    }
  }
}

QBrush
Mixer_HCTL_Tree_Model::get_foreground ( const QString & name_n )
{
  QBrush res;

  {
    QBrush * br_snd ( 0 );
    const QString & ename ( name_n );
    if ( ename.contains ( "playback", Qt::CaseInsensitive ) ) {
      br_snd = &_brush_snd_dir[ 0 ];
    } else if ( ename.contains ( "capture", Qt::CaseInsensitive ) ) {
      br_snd = &_brush_snd_dir[ 1 ];
    } else if ( ename.contains ( "input", Qt::CaseInsensitive ) ) {
      br_snd = &_brush_snd_dir[ 1 ];
    } else if ( ename.contains ( "mic", Qt::CaseInsensitive ) ) {
      br_snd = &_brush_snd_dir[ 1 ];
    }

    if ( br_snd != 0 ) {
      res = *br_snd;
    }
  }

  return res;
}

} // namespace MWdg
