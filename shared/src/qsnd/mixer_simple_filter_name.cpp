/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_simple_filter_name.hpp"
#include "qsnd/mixer_simple_elem.hpp"
#include <iostream>

namespace QSnd
{

Mixer_Simple_Filter_Name::Mixer_Simple_Filter_Name ( bool blacklist_n )
: _blacklist ( blacklist_n )
{
}

Mixer_Simple_Filter_Name::~Mixer_Simple_Filter_Name () {}

void
Mixer_Simple_Filter_Name::set_blacklist ( bool flag_n )
{
  _blacklist = flag_n;
}

void
Mixer_Simple_Filter_Name::append_name ( const QString & name_n )
{
  _names.append ( name_n );
}

void
Mixer_Simple_Filter_Name::append_names ( const QList< QString > & names_n )
{
  _names.append ( names_n );
}

unsigned int
Mixer_Simple_Filter_Name::filter (
    QList<::QSnd::Mixer_Simple_Elem * > & accept_n,
    QList<::QSnd::Mixer_Simple_Elem * > & drop_n )
{
  unsigned int cnt ( 0 );

  if ( _names.size () > 0 ) {
    int aii ( 0 );
    while ( aii < accept_n.size () ) {
      bool match ( false );
      ::QSnd::Mixer_Simple_Elem * elem ( accept_n[ aii ] );
      if ( elem != 0 ) {
        // Compare all names
        for ( int dii = 0; dii < _names.size (); ++dii ) {
          const QString & cmp_str ( _names[ dii ] );
          if ( ::QString::compare (
                   cmp_str, elem->elem_name (), Qt::CaseInsensitive ) == 0 ) {
            match = true;
          } else if ( ::QString::compare ( cmp_str,
                                           elem->display_name (),
                                           Qt::CaseInsensitive ) == 0 ) {
            match = true;
          }
          if ( match ) {
            break;
          }
        }
      }

      if ( match == blacklist () ) {
        // Drop element
        accept_n.removeAt ( aii );
        drop_n.append ( elem );
        ++cnt;
      } else {
        ++aii;
      }
    }
  }

  return cnt;
}

} // namespace QSnd
