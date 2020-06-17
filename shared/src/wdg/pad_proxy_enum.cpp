/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pad_proxy_enum.hpp"

namespace Wdg
{

Pad_Proxy_Enum::Pad_Proxy_Enum ()
: _enum_num_items ( 0 )
, _enum_index ( 0 )
{
}

void
Pad_Proxy_Enum::set_enum_num_items ( int num_n )
{
  _enum_num_items = num_n;
}

QString
Pad_Proxy_Enum::enum_item_name ( int )
{
  // Dummy implementation
  return QString ();
}

void
Pad_Proxy_Enum::set_enum_index ( int idx_n )
{
  if ( idx_n != enum_index () ) {
    _enum_index = idx_n;
    this->enum_index_changed ();
    emit sig_enum_index_changed ( enum_index () );
  }
}

void
Pad_Proxy_Enum::enum_index_changed ()
{
  // Dummy implementation
}

} // namespace Wdg
