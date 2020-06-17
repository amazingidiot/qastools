/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_pad_proxy_hpp__
#define __INC_wdg_pad_proxy_hpp__

#include <QObject>
#include <QPalette>
#include <QWidget>

// Forward declaration
namespace Wdg
{
class Pad_Proxy_Style;
class Pad_Proxies_Column;
class Pad_Proxies_Group;
} // namespace Wdg

namespace Wdg
{

/// @brief Pad_Proxy
///
class Pad_Proxy : public QObject
{
  Q_OBJECT;

  // Public methods
  public:
  Pad_Proxy ( unsigned char index_n = 0 );

  virtual ~Pad_Proxy ();

  ::Wdg::Pad_Proxies_Column *
  column () const;

  ::Wdg::Pad_Proxies_Group *
  group () const;

  // Position index

  /// @brief The position index in the parent column
  ///
  unsigned char
  index () const;

  void
  set_index ( unsigned char idx_n );

  // Focus flag

  bool
  has_focus () const;

  void
  set_has_focus ( bool flag_n );

  // Enabled flag

  bool
  is_enabled () const;

  void
  set_enabled ( bool flag_n );

  // Widget

  QWidget *
  widget () const;

  void
  set_widget ( QWidget * wdg_n );

  // Item name

  const QString &
  item_name () const;

  void
  set_item_name ( const QString & name_n );

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

  // Style information

  unsigned int
  variant_id () const;

  void
  set_variant_id ( unsigned int id_n );

  unsigned int
  style_id () const;

  void
  set_style_id ( unsigned int id_n );

  const ::Wdg::Pad_Proxy_Style *
  style () const;

  void
  set_style ( ::Wdg::Pad_Proxy_Style * style_n );

  bool
  eventFilter ( QObject * obj_n, QEvent * event_n );

  // Signals
  signals:

  void
  sig_enabled_changed ( bool flag_n );

  // Public slots
  public slots:

  virtual void
  update_value_from_source ();

  // Private attributes
  private:
  unsigned char _index;
  bool _has_focus;
  bool _is_enabled;

  QString _item_name;
  QString _group_name;
  QString _tool_tip;
  unsigned int _variant_id;
  unsigned int _style_id;

  QWidget * _widget;
  ::Wdg::Pad_Proxy_Style * _style;
};

inline unsigned char
Pad_Proxy::index () const
{
  return _index;
}

inline bool
Pad_Proxy::has_focus () const
{
  return _has_focus;
}

inline bool
Pad_Proxy::is_enabled () const
{
  return _is_enabled;
}

inline QWidget *
Pad_Proxy::widget () const
{
  return _widget;
}

inline const QString &
Pad_Proxy::item_name () const
{
  return _item_name;
}

inline const QString &
Pad_Proxy::group_name () const
{
  return _group_name;
}

inline const QString &
Pad_Proxy::tool_tip () const
{
  return _tool_tip;
}

inline unsigned int
Pad_Proxy::variant_id () const
{
  return _variant_id;
}

inline unsigned int
Pad_Proxy::style_id () const
{
  return _style_id;
}

inline const ::Wdg::Pad_Proxy_Style *
Pad_Proxy::style () const
{
  return _style;
}

} // namespace Wdg

#endif
