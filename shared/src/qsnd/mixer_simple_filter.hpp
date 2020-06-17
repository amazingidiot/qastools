/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_simple_filter_hpp__
#define __INC_mixer_simple_filter_hpp__

#include <QList>

// Forward declaration
namespace QSnd
{
class Mixer_Simple_Elem;
}

namespace QSnd
{

/// @brief Mixer_Simple_Filter
///
/// Filters mixer elements from a list
///
class Mixer_Simple_Filter
{
  // Public methods
  public:
  Mixer_Simple_Filter ();

  virtual ~Mixer_Simple_Filter ();

  /// @brief Filters certain elements from accent_n to drop_n
  ///
  /// @return The number of dropped elements
  virtual unsigned int
  filter ( QList<::QSnd::Mixer_Simple_Elem * > & accept_n,
           QList<::QSnd::Mixer_Simple_Elem * > & drop_n ) = 0;
};

} // namespace QSnd

#endif
