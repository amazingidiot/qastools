/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_pad_proxy_enum_hpp__
#define __INC_wdg_pad_proxy_enum_hpp__

#include "wdg/pad_proxy.hpp"

namespace Wdg
{

/// @brief Pad_Proxy_Enum
///
class Pad_Proxy_Enum : public ::Wdg::Pad_Proxy
{
  Q_OBJECT

  // Public methods
  public:
  Pad_Proxy_Enum ();

  int
  enum_num_items () const;

  void
  set_enum_num_items ( int num_n );

  virtual QString
  enum_item_name ( int idx_n );

  int
  enum_index () const;

  // Public slots
  public slots:

  void
  set_enum_index ( int idx_n );

  // Signals
  signals:

  void
  sig_enum_index_changed ( int value_n );

  // Protected methods
  protected:
  virtual void
  enum_index_changed ();

  // Private attributes
  private:
  int _enum_num_items;
  int _enum_index;
};

inline int
Pad_Proxy_Enum::enum_num_items () const
{
  return _enum_num_items;
}

inline int
Pad_Proxy_Enum::enum_index () const
{
  return _enum_index;
}

} // namespace Wdg

#endif
