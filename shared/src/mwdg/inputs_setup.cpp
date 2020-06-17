/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "inputs_setup.hpp"
#include <QCoreApplication>

namespace MWdg
{

Inputs_Setup::Inputs_Setup ()
{
  wheel_degrees = 720;
}

void
Inputs_Setup::update_translation ()
{
  // These key sequence should be all different!

  //: "s" is an abbreviation for "split" or "separate".
  //: Something like "j" for "joined" or "c" for "channel" may be appropriate,
  //too.
  ks_toggle_joined = QCoreApplication::translate ( "MWdg::Inputs_Setup", "s" );

  //: "l" is an abbreviation for "level"
  ks_level_channels = QCoreApplication::translate ( "MWdg::Inputs_Setup", "l" );

  //: "m" is an abbreviation for "mute"
  ks_mute_volumes = QCoreApplication::translate ( "MWdg::Inputs_Setup", "m" );

  //: "p" is an abbreviation for "playback"
  ks_toggle_vis_stream[ 0 ] =
      QCoreApplication::translate ( "MWdg::Inputs_Setup", "p" );

  //: "c" is an abbreviation for "capture" or "record"
  ks_toggle_vis_stream[ 1 ] =
      QCoreApplication::translate ( "MWdg::Inputs_Setup", "c" );

  // Strings

  //: Used in the context menu (slider/switch right click menu)
  ts_split_channels =
      QCoreApplication::translate ( "MWdg::Inputs_Setup", "Split &channels" );

  //: Used in the context menu (slider/switch right click menu)
  ts_join_channels =
      QCoreApplication::translate ( "MWdg::Inputs_Setup", "Join &channels" );

  //: Used in the context menu (slider/switch right click menu)
  ts_level_channels =
      QCoreApplication::translate ( "MWdg::Inputs_Setup", "&Level channels" );
}

} // namespace MWdg
