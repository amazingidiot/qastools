/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_alsa_config_model_hpp__
#define __INC_qsnd_alsa_config_model_hpp__

#include "qsnd/alsa.hpp"
#include "static_tree_model.hpp"

namespace QSnd
{

/// @brief Alsa_Config_Model
///
class Alsa_Config_Model : public Static_Tree_Model
{
  Q_OBJECT

  // Public methods
  public:
  Alsa_Config_Model ();

  ~Alsa_Config_Model ();

  // Model methods

  QVariant
  headerData ( int section,
               Qt::Orientation orientation,
               int role = Qt::DisplayRole ) const;

  Qt::ItemFlags
  flags ( const QModelIndex & index_n ) const;

  QVariant
  data ( const QModelIndex & index_n, int role = Qt::DisplayRole ) const;

  QString
  index_address_str ( const QModelIndex & index_n );

  QModelIndex
  index_from_address ( const QString & addr_n );

  // Public slots
  public slots:

  int
  reload ();

  // Private methods
  private:
  void
  clear_config ();

  int
  load_config ();

  int
  add_children_recursively ( Node * node_n, snd_config_t * cfg_n );

  snd_config_t *
  cfg_struct ( const Node * node_n ) const;

  snd_config_t *
  cfg_struct ( const QModelIndex & index_n ) const;

  int
  cfg_count_children ( snd_config_t * cfg_n ) const;

  snd_config_t *
  cfg_child ( snd_config_t * cfg_n, unsigned int index_n ) const;

  QString
  cfg_id_string ( snd_config_t * cfg_n ) const;

  QString
  cfg_value_string ( snd_config_t * cfg_n ) const;

  // Private attributes
  private:
  snd_config_t * _snd_cfg_root;
};

} // namespace QSnd

#endif
