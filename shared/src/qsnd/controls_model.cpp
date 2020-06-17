/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "controls_model.hpp"
#include "qsnd/alsa.hpp"
#include "qsnd/controls_database.hpp"
#include "qsnd/ctl_format.hpp"
#include <QCoreApplication>
#include <QFont>
#include <iostream>

namespace QSnd
{

Controls_Model::Controls_Model ( QObject * parent_n )
: QAbstractListModel ( parent_n )
{
}

Controls_Model::~Controls_Model () {}

void
Controls_Model::set_controls_db ( ::QSnd::Controls_Database * ctl_db_n )
{
  if ( _ctl_db == ctl_db_n ) {
    return;
  }

  reload_begin ();
  {
    if ( _ctl_db != nullptr ) {
      disconnect ( _ctl_db, 0, this, 0 );
    }
    _ctl_db = ctl_db_n;
    if ( _ctl_db != nullptr ) {
      connect ( _ctl_db,
                &QSnd::Controls_Database::sig_reload_begin,
                this,
                &Controls_Model::reload_begin );
      connect ( _ctl_db,
                &QSnd::Controls_Database::sig_reload_end,
                this,
                &Controls_Model::reload_finish );
    }
  }
  reload_finish ();
}

const ::QSnd::CTL_Format *
Controls_Model::control_by_index ( const QModelIndex & idx_n ) const
{
  if ( ( _ctl_db == nullptr ) || ( !idx_n.isValid () ) ||
       ( idx_n.parent ().isValid () ) || ( idx_n.column () != 0 ) ||
       ( idx_n.row () < 0 ) ||
       ( idx_n.row () >= static_cast< int > ( _controls.size () ) ) ) {
    return nullptr;
  }

  return _controls[ idx_n.row () ].get ();
}

QModelIndex
Controls_Model::control_index ( const QString & ctl_addr_n ) const
{
  int row = 0;
  for ( const auto & item : _controls ) {
    if ( item->match ( ctl_addr_n ) ) {
      return index ( row, 0 );
    }
    ++row;
  }
  return QModelIndex ();
}

void
Controls_Model::reload ()
{
  reload_begin ();
  reload_finish ();
}

void
Controls_Model::reload_begin ()
{
  beginResetModel ();
}

void
Controls_Model::reload_finish ()
{
  load_data ();
  endResetModel ();
  emit countChanged ();
}

void
Controls_Model::load_data ()
{
  if ( _ctl_db == nullptr ) {
    return;
  }
  if ( _ctl_db->num_controls () == 0 ) {
    return;
  }

  // Copy controls
  _controls = _ctl_db->controls ();

  // Sort controls
  using Const_Handle = Controls_Database::Const_Handle;
  std::sort ( _controls.begin (),
              _controls.end (),
              [] ( const Const_Handle & a_n, const Const_Handle & b_n ) {
                return a_n->ctl_name () < b_n->ctl_name ();
              } );
}

QHash< int, QByteArray >
Controls_Model::roleNames () const
{
  auto res = QAbstractListModel::roleNames ();
  res.insert ( ROLE_L10N_ARGS, "argsL10N" );
  return res;
}

int
Controls_Model::rowCount ( const QModelIndex & parent_n ) const
{
  if ( parent_n.isValid () ) {
    return 0;
  }
  return _controls.size ();
}

QVariant
Controls_Model::data ( const QModelIndex & index_n, int role_n ) const
{
  auto control = control_by_index ( index_n );
  if ( !control ) {
    return QVariant ();
  }

  switch ( role_n ) {
  case Qt::DisplayRole:
    return QVariant ( control->ctl_name () );

  case Qt::ToolTipRole: {
    QString ttip ( control->ctl_name () );
    for ( std::size_t ii = 0; ii != control->num_args (); ++ii ) {
      if ( ii == 0 ) {
        ttip.append ( ":" );
      } else {
        ttip.append ( "," );
      }
      ttip.append ( control->arg ( ii ).arg_name );
    }
    return QVariant ( ttip );
  }

  case ROLE_L10N_ARGS: {
    QStringList args_l10n;
    for ( std::size_t ii = 0; ii != control->num_args (); ++ii ) {
      args_l10n.append ( QCoreApplication::translate (
          "ALSA::CTL_Arg_Name",
          control->arg ( ii ).arg_name.toUtf8 ().constData () ) );
    }
    return QVariant ( args_l10n );
  }

  default:
    break;
  }

  return QVariant ();
}

} // namespace QSnd
