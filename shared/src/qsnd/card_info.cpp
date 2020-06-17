/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "card_info.hpp"
#include <iostream>

namespace QSnd
{

Card_Info::Card_Info () {}

Card_Info::Card_Info ( const int hw_idx_n )
{
  acquire_info ( hw_idx_n );
}

Card_Info::Card_Info ( const QString & dev_str_n )
{
  acquire_info ( dev_str_n );
}

void
Card_Info::clear ()
{
  _index = -1;
  _id.clear ();
  _driver.clear ();
  _name.clear ();
  _long_name.clear ();
  _mixer_name.clear ();
  _components.clear ();
}

int
Card_Info::acquire_info ( const int hw_idx_n )
{
  QString dev_str ( "hw:%1" );
  dev_str = dev_str.arg ( hw_idx_n );

  return acquire_info ( dev_str );
}

int
Card_Info::acquire_info ( const QString & dev_str_n )
{
  // Open control handle
  snd_hctl_t * snd_hctl = nullptr;
  {
    int oerr = snd_hctl_open (
        &snd_hctl, dev_str_n.toLocal8Bit ().constData (), SND_CTL_NONBLOCK );
    if ( oerr < 0 ) {
      return oerr;
    }
  }

  int ierr = acquire_info ( snd_hctl );
  snd_hctl_close ( snd_hctl );
  return ierr;
}

int
Card_Info::acquire_info ( snd_hctl_t * snd_hctl_n )
{
  snd_ctl_t * snd_card_ctl = nullptr;
  snd_ctl_card_info_t * snd_card_info = nullptr;

  snd_ctl_card_info_alloca ( &snd_card_info );
  snd_card_ctl = snd_hctl_ctl ( snd_hctl_n );

  // Get card information
  int err = snd_ctl_card_info ( snd_card_ctl, snd_card_info );
  if ( err < 0 ) {
    clear ();
    return err;
  }

  _index = snd_ctl_card_info_get_card ( snd_card_info );
  _id = snd_ctl_card_info_get_id ( snd_card_info );
  _driver = snd_ctl_card_info_get_driver ( snd_card_info );
  _name = snd_ctl_card_info_get_name ( snd_card_info );
  _long_name = snd_ctl_card_info_get_longname ( snd_card_info );
  _mixer_name = snd_ctl_card_info_get_mixername ( snd_card_info );
  _components = snd_ctl_card_info_get_components ( snd_card_info );

  return err;
}

bool
Card_Info::operator== ( const ::QSnd::Card_Info & cinfo_n ) const
{
  return ( _index == cinfo_n._index ) && ( _id == cinfo_n._id ) &&
         ( _driver == cinfo_n._driver ) && ( _name == cinfo_n._name ) &&
         ( _long_name == cinfo_n._long_name ) &&
         ( _mixer_name == cinfo_n._mixer_name ) &&
         ( _components == cinfo_n._components );
}

bool
Card_Info::operator!= ( const ::QSnd::Card_Info & cinfo_n ) const
{
  return !operator== ( cinfo_n );
}

} // namespace QSnd
