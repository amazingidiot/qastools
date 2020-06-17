/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_simple_filter_name_hpp__
#define __INC_qsnd_mixer_simple_filter_name_hpp__

#include "qsnd/mixer_simple_filter.hpp"

namespace QSnd
{

/// @brief Mixer_Simple_Filter
///
/// Filters mixer elements from a list by their name
///
class Mixer_Simple_Filter_Name : public ::QSnd::Mixer_Simple_Filter
{
  // Public methods
  public:
  Mixer_Simple_Filter_Name ( bool blacklist_n = true );

  virtual ~Mixer_Simple_Filter_Name ();

  /// @brief true if this is a blacklist. false if it is a whitelist.
  ///
  bool
  blacklist () const;

  void
  set_blacklist ( bool flag_n );

  void
  append_name ( const QString & name_n );

  void
  append_names ( const QList< QString > & names_n );

  unsigned int
  filter ( QList<::QSnd::Mixer_Simple_Elem * > & accept_n,
           QList<::QSnd::Mixer_Simple_Elem * > & drop_n );

  // Private attributes
  private:
  QList< QString > _names;
  bool _blacklist;
};

inline bool
Mixer_Simple_Filter_Name::blacklist () const
{
  return _blacklist;
}

} // namespace QSnd

#endif
