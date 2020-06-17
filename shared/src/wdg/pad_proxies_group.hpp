/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_pad_proxies_group_hpp__
#define __INC_wdg_pad_proxies_group_hpp__

#include <QList>
#include <QObject>

// Forward declaration
namespace Wdg
{
class Pad_Proxies_Column;
}

namespace Wdg
{

class Pad_Proxies_Group : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Pad_Proxies_Group ( QObject * parent_n = 0 );

  virtual ~Pad_Proxies_Group ();

  /// @brief The pad object
  /// Gets set by the pad and shouldn't be set manually
  QObject *
  pad () const;

  void
  set_pad ( QObject * pad_n );

  // Group index

  /// @brief The group index
  /// Gets set by the sliders pad and shouldn't be set manually
  unsigned int
  group_index () const;

  void
  set_group_index ( unsigned int idx_n );

  // Group name

  const QString &
  group_name () const;

  void
  set_group_name ( const QString & name_n );

  // Tool tip

  const QString &
  tool_tip () const;

  void
  set_tool_tip ( const QString & tip_n );

  // Style id

  unsigned int
  style_id () const;

  void
  set_style_id ( unsigned int style_id_n );

  // Proxies columns

  unsigned int
  num_columns () const;

  void
  append_column ( ::Wdg::Pad_Proxies_Column * column_n );

  void
  clear_columns ();

  ::Wdg::Pad_Proxies_Column *
  column ( unsigned int idx_n ) const;

  // Statistics

  unsigned int
  num_sliders () const;

  unsigned int
  num_switches () const;

  // Focus

  unsigned char
  focus_column () const;

  unsigned char
  focus_row () const;

  bool
  has_focus () const;

  /// @return true when the focus was taken - false when already has focus or
  /// invalid column
  bool
  take_focus ( unsigned int column_n, unsigned int row_n );

  // Protected methods
  protected:
  bool
  event ( QEvent * event_n );

  // Private attributes
  private:
  QObject * _pad;
  unsigned int _group_index;
  unsigned int _style_id;

  QList<::Wdg::Pad_Proxies_Column * > _columns;

  QString _group_name;
  QString _tool_tip;

  unsigned int _num_sliders;
  unsigned int _num_switches;
  unsigned char _focus_column;
  unsigned char _focus_row;
  bool _has_focus;
};

inline QObject *
Pad_Proxies_Group::pad () const
{
  return _pad;
}

inline unsigned int
Pad_Proxies_Group::group_index () const
{
  return _group_index;
}

inline unsigned int
Pad_Proxies_Group::num_columns () const
{
  return _columns.size ();
}

inline ::Wdg::Pad_Proxies_Column *
Pad_Proxies_Group::column ( unsigned int idx_n ) const
{
  return _columns[ idx_n ];
}

inline const QString &
Pad_Proxies_Group::group_name () const
{
  return _group_name;
}

inline const QString &
Pad_Proxies_Group::tool_tip () const
{
  return _tool_tip;
}

inline unsigned int
Pad_Proxies_Group::style_id () const
{
  return _style_id;
}

inline unsigned int
Pad_Proxies_Group::num_sliders () const
{
  return _num_sliders;
}

inline unsigned int
Pad_Proxies_Group::num_switches () const
{
  return _num_switches;
}

inline unsigned char
Pad_Proxies_Group::focus_column () const
{
  return _focus_column;
}

inline unsigned char
Pad_Proxies_Group::focus_row () const
{
  return _focus_row;
}

inline bool
Pad_Proxies_Group::has_focus () const
{
  return _has_focus;
}

} // namespace Wdg

#endif
