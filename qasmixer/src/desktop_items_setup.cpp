/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "desktop_items_setup.hpp"
#include <QSettings>
#include <iostream>

Desktop_Items_Setup::Desktop_Items_Setup ()
: start_device_mode ( ::Desktop_Items_Setup::MIXER_DEV_DEFAULT )
, start_user_device ( "hw:0" )
, tray_on_close ( true )
, tray_show_icon ( true )
, tray_is_minimized ( false )
{
}

void
Desktop_Items_Setup::read_from_storage ()
{
  Main_Window_Setup & mwin ( main_window );

  QSettings settings;

  start_device_mode =
      settings.value ( "start_device_mode", start_device_mode ).toUInt ();

  start_user_device =
      settings.value ( "start_user_device", start_user_device ).toString ();

  mwin.mixer_dev.ctl_addr =
      settings.value ( "current_device", mwin.mixer_dev.ctl_addr ).toString ();

  mwin.inputs.wheel_degrees =
      settings.value ( "wheel_degrees", mwin.inputs.wheel_degrees ).toUInt ();

  tray_on_close = settings.value ( "tray_on_close", tray_on_close ).toBool ();

  tray_show_icon =
      settings.value ( "tray_show_icon", tray_show_icon ).toBool ();

  tray_is_minimized =
      settings.value ( "tray_is_minimized", tray_is_minimized ).toBool ();

  // Main window state
  {
    settings.beginGroup ( "main_window" );

    mwin.window_state =
        settings.value ( "window_state", mwin.window_state ).toByteArray ();

    mwin.window_geometry =
        settings.value ( "window_geometry", mwin.window_geometry )
            .toByteArray ();

    mwin.splitter_state =
        settings.value ( "splitter_state", mwin.splitter_state ).toByteArray ();

    mwin.show_dev_select =
        settings.value ( "show_device_selection", mwin.show_dev_select )
            .toBool ();

    settings.endGroup ();
  }

  // Device selection
  {
    ::Views::Device_Selection_View_Setup & vsetup ( main_window.dev_select );

    settings.beginGroup ( "device_selection" );

    vsetup.selection_db_set (
        settings.value ( "selection_db", vsetup.selection_db_get () )
            .toStringList () );

    settings.endGroup ();
  }

  // Simple mixer
  {
    ::Views::Mixer_Simple_Setup & vsetup ( main_window.mixer_simple );

    settings.beginGroup ( "simple_mixer" );

    vsetup.show_stream[ 0 ] =
        settings.value ( "show_stream_playback", vsetup.show_stream[ 0 ] )
            .toBool ();

    vsetup.show_stream[ 1 ] =
        settings.value ( "show_stream_capture", vsetup.show_stream[ 1 ] )
            .toBool ();

    vsetup.show_slider_value_labels =
        settings
            .value ( "show_slider_value_labels",
                     vsetup.show_slider_value_labels )
            .toBool ();

    settings.endGroup ();
  }

  // Tray mixer
  {
    settings.beginGroup ( "tray_mixer" );

    Tray_Mixer_MDev_Setup & dsetup ( tray_mdev );

    dsetup.device_mode =
        settings.value ( "device_mode", dsetup.device_mode ).toInt ();

    dsetup.user_device =
        settings.value ( "user_device", dsetup.user_device ).toString ();

    Tray_Mixer_View_Setup & vsetup ( tray_view );

    vsetup.show_balloon =
        settings.value ( "show_balloon", vsetup.show_balloon ).toBool ();

    vsetup.balloon_lifetime =
        settings.value ( "balloon_lifetime", vsetup.balloon_lifetime )
            .toUInt ();

    settings.endGroup ();
  }

  // Settings view
  {
    settings.beginGroup ( "settings_dialog" );

    settings_dialog.page =
        settings.value ( "page", settings_dialog.page ).toUInt ();

    settings.endGroup ();
  }

  // Sanitize values

  if ( main_window.inputs.wheel_degrees == 0 ) {
    main_window.inputs.wheel_degrees = 720;
  }
  tray_view.wheel_degrees = main_window.inputs.wheel_degrees;

  if ( start_device_mode > ::Desktop_Items_Setup::MIXER_DEV_LAST ) {
    start_device_mode = ::Desktop_Items_Setup::MIXER_DEV_DEFAULT;
  }

  if ( tray_mdev.device_mode > ::Tray_Mixer_MDev_Setup::MIXER_DEV_LAST ) {
    tray_mdev.device_mode = ::Tray_Mixer_MDev_Setup::MIXER_DEV_DEFAULT;
  }

  {
    ::Views::Mixer_Simple_Setup & vsetup ( main_window.mixer_simple );
    if ( !( vsetup.show_stream[ 0 ] || vsetup.show_stream[ 1 ] ) ) {
      vsetup.show_stream[ 0 ] = true;
    }
  }
}

void
Desktop_Items_Setup::write_to_storage ()
{
  //::std::cout << "Desktop_Items_Setup::write_to_storage" << "\n";

  QSettings settings;

  // General

  settings.setValue ( "start_device_mode", start_device_mode );

  settings.setValue ( "start_user_device", start_user_device );

  settings.setValue ( "current_device", main_window.mixer_dev.ctl_addr );

  settings.setValue ( "wheel_degrees", main_window.inputs.wheel_degrees );

  settings.setValue ( "tray_on_close", tray_on_close );

  settings.setValue ( "tray_show_icon", tray_show_icon );

  settings.setValue ( "tray_is_minimized", tray_is_minimized );

  // Main window state
  {
    settings.beginGroup ( "main_window" );

    settings.setValue ( "window_state", main_window.window_state );

    settings.setValue ( "window_geometry", main_window.window_geometry );

    settings.setValue ( "splitter_state", main_window.splitter_state );

    settings.setValue ( "show_device_selection", main_window.show_dev_select );

    settings.endGroup ();
  }

  // Device selection
  {
    const ::Views::Device_Selection_View_Setup & vsetup (
        main_window.dev_select );
    settings.beginGroup ( "device_selection" );

    settings.setValue ( "selection_db", vsetup.selection_db_get () );

    settings.endGroup ();
  }

  // Simple mixer
  {
    const ::Views::Mixer_Simple_Setup & vsetup ( main_window.mixer_simple );

    settings.beginGroup ( "simple_mixer" );

    settings.setValue ( "show_stream_playback", vsetup.show_stream[ 0 ] );

    settings.setValue ( "show_stream_capture", vsetup.show_stream[ 1 ] );

    settings.setValue ( "show_slider_value_labels",
                        vsetup.show_slider_value_labels );

    settings.endGroup ();
  }

  // Mini mixer
  {

    settings.beginGroup ( "tray_mixer" );

    const Tray_Mixer_MDev_Setup & dsetup ( tray_mdev );

    settings.setValue ( "device_mode", dsetup.device_mode );

    settings.setValue ( "user_device", dsetup.user_device );

    const Tray_Mixer_View_Setup & vsetup ( tray_view );

    settings.setValue ( "show_balloon", vsetup.show_balloon );

    settings.setValue ( "balloon_lifetime", vsetup.balloon_lifetime );

    settings.endGroup ();
  }

  // Settings view
  {
    settings.beginGroup ( "settings_dialog" );

    settings.setValue ( "page", settings_dialog.page );

    settings.endGroup ();
  }
}
