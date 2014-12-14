
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8.11 )


# --- Find modules

if ( USE_QT5 )
FIND_PACKAGE ( Qt5Core REQUIRED )
FIND_PACKAGE ( Qt5Widgets REQUIRED )
FIND_PACKAGE ( Qt5Svg REQUIRED )
FIND_PACKAGE ( Qt5Network REQUIRED )
else()
SET ( QT_USE_QTCORE TRUE )
SET ( QT_USE_QTGUI TRUE )
SET ( QT_USE_QTSVG TRUE )
SET ( QT_USE_QTNETWORK TRUE )
FIND_PACKAGE ( Qt4 4.6 REQUIRED )

INCLUDE ( ${QT_USE_FILE} )
endif()
FIND_PACKAGE ( ALSA REQUIRED )

# TODO: FIND_PACKAGE
SET ( UDEV_LIBRARIES udev )


# --- Configure files

INCLUDE ( CheckIncludeFiles )
CONFIGURE_FILE (
	${SHARED_SRC_DIR}/qastools_config.hpp.cmake
	${CMAKE_CURRENT_BINARY_DIR}/qastools_config.hpp )


# --- Sources

SET ( SRC_GUI

	${SHARED_SRC_DIR}/qsnd/alsa.cpp
	${SHARED_SRC_DIR}/qsnd/event_types.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_format_argument.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_format.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_address_argument.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_address.cpp
	${SHARED_SRC_DIR}/qsnd/controls_database.cpp
	${SHARED_SRC_DIR}/qsnd/controls_model.cpp
	${SHARED_SRC_DIR}/qsnd/cards_model.cpp
	${SHARED_SRC_DIR}/qsnd/card_info.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple_elem.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple_filter.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple_filter_name.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple.cpp
	${SHARED_SRC_DIR}/qsnd/udev_device_lookout.cpp

	${SHARED_SRC_DIR}/dpe/image.cpp
	${SHARED_SRC_DIR}/dpe/image_set_meta.cpp
	${SHARED_SRC_DIR}/dpe/image_set.cpp
	${SHARED_SRC_DIR}/dpe/image_set_group.cpp
	${SHARED_SRC_DIR}/dpe/image_set_state.cpp
	${SHARED_SRC_DIR}/dpe/image_request.cpp
	${SHARED_SRC_DIR}/dpe/image_allocator.cpp
	${SHARED_SRC_DIR}/dpe/is_buffer_handle.cpp
	${SHARED_SRC_DIR}/dpe/is_buffer.cpp
	${SHARED_SRC_DIR}/dpe/paint_job.cpp
	${SHARED_SRC_DIR}/dpe/painter.cpp
	${SHARED_SRC_DIR}/dpe/painter_simple.cpp
	${SHARED_SRC_DIR}/dpe/painter_thread.cpp
	${SHARED_SRC_DIR}/dpe/painter_thread_shared.cpp

	${SHARED_SRC_DIR}/wdg/label_elide.cpp
	${SHARED_SRC_DIR}/wdg/label_width.cpp
	${SHARED_SRC_DIR}/wdg/text_browser.cpp
	${SHARED_SRC_DIR}/wdg/scroll_area_horizontal.cpp
	${SHARED_SRC_DIR}/wdg/scroll_area_vertical.cpp
	${SHARED_SRC_DIR}/wdg/balloon_widget.cpp
	${SHARED_SRC_DIR}/wdg/color_methods.cpp
	${SHARED_SRC_DIR}/wdg/ds_widget_style_db.cpp
	${SHARED_SRC_DIR}/wdg/ds_widget_painter.cpp
	${SHARED_SRC_DIR}/wdg/ds_imaging.cpp
	${SHARED_SRC_DIR}/wdg/ds_switch_painter_circle.cpp
	${SHARED_SRC_DIR}/wdg/ds_switch_painter_close.cpp
	${SHARED_SRC_DIR}/wdg/ds_switch_painter_svg.cpp
	${SHARED_SRC_DIR}/wdg/ds_switch.cpp

	${SHARED_SRC_DIR}/wdg/uint_mapper.cpp
	${SHARED_SRC_DIR}/wdg/cubic_curve.cpp
	${SHARED_SRC_DIR}/wdg/ds_slider_meta_bg.cpp
	${SHARED_SRC_DIR}/wdg/ds_slider_painter_bevelled.cpp
	${SHARED_SRC_DIR}/wdg/ds_slider.cpp

	${SHARED_SRC_DIR}/wdg/event_types.cpp
	${SHARED_SRC_DIR}/wdg/pass_events.cpp

	${SHARED_SRC_DIR}/wdg/pad_focus_info.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxy.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxy_style.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxy_slider.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxy_switch.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxy_enum.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxies_column.cpp
	${SHARED_SRC_DIR}/wdg/pad_proxies_group.cpp

	${SHARED_SRC_DIR}/wdg/layout_weights.cpp
	${SHARED_SRC_DIR}/wdg/equal_columns_layout_group.cpp
	${SHARED_SRC_DIR}/wdg/equal_columns_layout.cpp
	${SHARED_SRC_DIR}/wdg/fill_columns_layout.cpp

	${SHARED_SRC_DIR}/wdg/sliders_pad_layout.cpp
	${SHARED_SRC_DIR}/wdg/sliders_pad_style.cpp
	${SHARED_SRC_DIR}/wdg/sliders_pad_data.cpp
	${SHARED_SRC_DIR}/wdg/sliders_pad_header_data.cpp
	${SHARED_SRC_DIR}/wdg/sliders_pad_header.cpp
	${SHARED_SRC_DIR}/wdg/sliders_pad_footer.cpp
	${SHARED_SRC_DIR}/wdg/sliders_pad.cpp

	${SHARED_SRC_DIR}/wdg/switches_pad_widgets.cpp
	${SHARED_SRC_DIR}/wdg/switches_pad_widgets_group.cpp
	${SHARED_SRC_DIR}/wdg/switches_pad.cpp
	${SHARED_SRC_DIR}/wdg/switches_area.cpp

	${SHARED_SRC_DIR}/mwdg/event_types.cpp
	${SHARED_SRC_DIR}/mwdg/ctl_arg_view.cpp
	${SHARED_SRC_DIR}/mwdg/ctl_arg_view_string.cpp
	${SHARED_SRC_DIR}/mwdg/ctl_arg_view_integer.cpp
	${SHARED_SRC_DIR}/mwdg/ctl_arg_view_card.cpp
	${SHARED_SRC_DIR}/mwdg/controls_delegate.cpp
	${SHARED_SRC_DIR}/mwdg/controls_view.cpp
	${SHARED_SRC_DIR}/mwdg/slider_status_widget.cpp
	${SHARED_SRC_DIR}/mwdg/inputs_setup.cpp
	${SHARED_SRC_DIR}/mwdg/mixer_device_setup.cpp
	${SHARED_SRC_DIR}/mwdg/mixer_style.cpp

	${SHARED_SRC_DIR}/views/view_utility.cpp
	${SHARED_SRC_DIR}/views/message_widget.cpp
	${SHARED_SRC_DIR}/views/view_base_setup.cpp
	${SHARED_SRC_DIR}/views/view_base.cpp
	${SHARED_SRC_DIR}/views/device_selection_view_setup.cpp
	${SHARED_SRC_DIR}/views/device_selection_view.cpp
	${SHARED_SRC_DIR}/views/basic_dialog.cpp
	${SHARED_SRC_DIR}/views/multi_page_dialog.cpp
	${SHARED_SRC_DIR}/views/info_dialog.cpp

	${SHARED_SRC_DIR}/unix_signal_handler.cpp
	${SHARED_SRC_DIR}/single_application.cpp
	${SHARED_SRC_DIR}/license_texts.cpp

	mwdg/mixer_gui_state.cpp
	mwdg/mixer_simple_setup.cpp
	mwdg/mixer_separation_info.cpp
	mwdg/mixer_sliders_proxy_slider.cpp
	mwdg/mixer_sliders_proxy_switch.cpp
	mwdg/mixer_sliders_proxies_column.cpp
	mwdg/mixer_sliders_proxies_group.cpp
	mwdg/mixer_sliders_status_widget.cpp
	mwdg/mixer_sliders.cpp
	mwdg/mixer_switches_proxy_enum.cpp
	mwdg/mixer_switches_proxy_switch.cpp
	mwdg/mixer_switches_proxies_group.cpp
	mwdg/mixer_switches.cpp

	views/mixer_simple_setup.cpp
	views/mixer_simple.cpp
	views/settings_dialog_setup.cpp
	views/settings_dialog.cpp

	tray_mixer_mdev_setup.cpp
	tray_mixer_view_setup.cpp
	tray_mixer_icon.cpp
	tray_mixer_balloon.cpp
	tray_mixer.cpp
	cmd_options.cpp
	main_window_setup.cpp
	main_window.cpp
	desktop_items_setup.cpp
	desktop_items.cpp

	init_globals.cpp
	info_texts.cpp
	main.cpp
)


SET ( SRC_GUI_HEADER_MOC

	${SHARED_INC_DIR}/qsnd/controls_database.hpp
	${SHARED_INC_DIR}/qsnd/controls_model.hpp
	${SHARED_INC_DIR}/qsnd/cards_model.hpp
	${SHARED_INC_DIR}/qsnd/mixer_simple_elem.hpp
	${SHARED_INC_DIR}/qsnd/mixer_simple.hpp
	${SHARED_INC_DIR}/qsnd/udev_device_lookout.hpp

	${SHARED_INC_DIR}/dpe/is_buffer.hpp
	${SHARED_INC_DIR}/dpe/image_allocator.hpp

	${SHARED_INC_DIR}/wdg/ds_slider.hpp
	${SHARED_INC_DIR}/wdg/ds_switch.hpp
	${SHARED_INC_DIR}/wdg/balloon_widget.hpp
	${SHARED_INC_DIR}/wdg/pad_proxy.hpp
	${SHARED_INC_DIR}/wdg/pad_proxy_slider.hpp
	${SHARED_INC_DIR}/wdg/pad_proxy_switch.hpp
	${SHARED_INC_DIR}/wdg/pad_proxy_enum.hpp
	${SHARED_INC_DIR}/wdg/pad_proxies_column.hpp
	${SHARED_INC_DIR}/wdg/pad_proxies_group.hpp
	${SHARED_INC_DIR}/wdg/sliders_pad_data.hpp
	${SHARED_INC_DIR}/wdg/sliders_pad_header.hpp
	${SHARED_INC_DIR}/wdg/sliders_pad_footer.hpp
	${SHARED_INC_DIR}/wdg/sliders_pad.hpp
	${SHARED_INC_DIR}/wdg/switches_pad.hpp

	${SHARED_INC_DIR}/mwdg/controls_delegate.hpp
	${SHARED_INC_DIR}/mwdg/controls_view.hpp
	${SHARED_INC_DIR}/mwdg/ctl_arg_view.hpp
	${SHARED_INC_DIR}/mwdg/ctl_arg_view_string.hpp
	${SHARED_INC_DIR}/mwdg/ctl_arg_view_integer.hpp
	${SHARED_INC_DIR}/mwdg/ctl_arg_view_card.hpp
	${SHARED_INC_DIR}/mwdg/slider_status_widget.hpp

	${SHARED_INC_DIR}/views/message_widget.hpp
	${SHARED_INC_DIR}/views/view_base.hpp
	${SHARED_INC_DIR}/views/device_selection_view.hpp
	${SHARED_INC_DIR}/views/basic_dialog.hpp
	${SHARED_INC_DIR}/views/multi_page_dialog.hpp
	${SHARED_INC_DIR}/views/info_dialog.hpp

	${SHARED_INC_DIR}/unix_signal_handler.hpp
	${SHARED_INC_DIR}/single_application.hpp

	mwdg/mixer_sliders_proxy_slider.hpp
	mwdg/mixer_sliders_proxy_switch.hpp
	mwdg/mixer_sliders_proxies_column.hpp
	mwdg/mixer_sliders_proxies_group.hpp
	mwdg/mixer_sliders_status_widget.hpp
	mwdg/mixer_sliders.hpp
	mwdg/mixer_switches_proxy_enum.hpp
	mwdg/mixer_switches_proxy_switch.hpp
	mwdg/mixer_switches_proxies_group.hpp
	mwdg/mixer_switches.hpp

	views/mixer_simple.hpp
	views/settings_dialog.hpp

	tray_mixer_icon.hpp
	tray_mixer_balloon.hpp
	tray_mixer.hpp
	main_window.hpp
	desktop_items.hpp
)


# --- Compiler flags

SET ( CXX_COMMON_FLAGS "-std=c++11 -fPIC -fPIE -Wextra -Wunused -Wall" )
SET ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-inline ${CXX_COMMON_FLAGS}" )
SET ( CMAKE_CXX_FLAGS_RELEASE "-O2 ${CXX_COMMON_FLAGS} -Wl,--as-needed" )


# --- Include directories

INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES ( ${SHARED_INC_DIR} )
INCLUDE_DIRECTORIES ( ${ALSA_INCLUDE_DIRS} )


# --- QT extras

if ( USE_QT5 )
QT5_WRAP_CPP ( SRC_GUI_MOC ${SRC_GUI_HEADER_MOC} )
QT5_WRAP_CPP ( SRC_EXTRA_MOC ${SRC_EXTRA_HEADER_MOC} )
else()
QT4_WRAP_CPP ( SRC_GUI_MOC ${SRC_GUI_HEADER_MOC} )
QT4_WRAP_CPP ( SRC_EXTRA_MOC ${SRC_EXTRA_HEADER_MOC} )
endif()


# --- Executable

ADD_EXECUTABLE ( ${PROGRAM_NAME}
	${SRC_GUI}
	${SRC_GUI_MOC}
)
if ( USE_QT5 )
TARGET_LINK_LIBRARIES ( ${PROGRAM_NAME}
	Qt5::Widgets
        Qt5::Svg
        Qt5::Network
	${ALSA_LIBRARIES}
	${UDEV_LIBRARIES}
)
else()
TARGET_LINK_LIBRARIES ( ${PROGRAM_NAME}
	${QT_LIBRARIES}
	${ALSA_LIBRARIES}
	${UDEV_LIBRARIES}
)
endif()


# --- Installation

INSTALL (
	TARGETS ${PROGRAM_NAME} RUNTIME
	DESTINATION ${INSTALL_DIR_BIN} )
