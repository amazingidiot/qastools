
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )


# Find modules
SET ( QT_USE_QTCORE TRUE )
SET ( QT_USE_QTGUI TRUE )
SET ( QT_USE_QTSVG TRUE )
SET ( QT_USE_QTOPENGL TRUE )
SET ( QT_USE_QTNETWORK TRUE )
FIND_PACKAGE ( Qt4 4.6 REQUIRED )
FIND_PACKAGE ( ALSA REQUIRED )
INCLUDE ( ${QT_USE_FILE} )


# Configure files
INCLUDE ( CheckIncludeFiles )
CONFIGURE_FILE (
	${SHARED_SRC_DIR}/qastools_config.hpp.cmake
	${CMAKE_CURRENT_BINARY_DIR}/qastools_config.hpp )


# Compiler flags
ADD_DEFINITIONS ( "-Wextra -Wunused -Wall" )
SET ( CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG -fno-inline" )
SET ( CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -Wl,--as-needed" )


# Sources

SET ( SRC_GUI

	${SHARED_SRC_DIR}/qsnd/alsa.cpp
	${SHARED_SRC_DIR}/qsnd/event_types.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_def_arg.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_def.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_address_arg.cpp
	${SHARED_SRC_DIR}/qsnd/ctl_address.cpp
	${SHARED_SRC_DIR}/qsnd/controls_db.cpp
	${SHARED_SRC_DIR}/qsnd/controls_model.cpp
	${SHARED_SRC_DIR}/qsnd/cards_model.cpp
	${SHARED_SRC_DIR}/qsnd/card_info.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple_elem.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple_filter.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple_filter_name.cpp
	${SHARED_SRC_DIR}/qsnd/mixer_simple.cpp

	${SHARED_SRC_DIR}/qsnd2/controls_proxies.cpp
	${SHARED_SRC_DIR}/qsnd2/controls.cpp
	${SHARED_SRC_DIR}/qsnd2/controls_watcher.cpp
	${SHARED_SRC_DIR}/qsnd2/alsa_smi.cpp

	${SHARED_SRC_DIR}/dpe2/key_values.cpp
	${SHARED_SRC_DIR}/dpe2/pixmap.cpp
	${SHARED_SRC_DIR}/dpe2/pixmap_request.cpp
	${SHARED_SRC_DIR}/dpe2/pixmap_ref.cpp
	${SHARED_SRC_DIR}/dpe2/pixmap_server.cpp
	${SHARED_SRC_DIR}/dpe2/pixmap_server_shared.cpp
	${SHARED_SRC_DIR}/dpe2/paint_thread.cpp
	${SHARED_SRC_DIR}/dpe2/painter.cpp
	${SHARED_SRC_DIR}/dpe2/painter_simple.cpp

	${SHARED_SRC_DIR}/wdg2/int_math.cpp
	${SHARED_SRC_DIR}/wdg2/sliders_pad.cpp
	${SHARED_SRC_DIR}/wdg2/slider_value_map.cpp
	${SHARED_SRC_DIR}/wdg2/graphical_widget.cpp
	${SHARED_SRC_DIR}/wdg2/gw_pixmaps.cpp
	${SHARED_SRC_DIR}/wdg2/gw_slider.cpp
	${SHARED_SRC_DIR}/wdg2/gw_multi_slider.cpp
	${SHARED_SRC_DIR}/wdg2/gw_switch.cpp
	${SHARED_SRC_DIR}/wdg2/gw_multi_switch.cpp
	${SHARED_SRC_DIR}/wdg2/gw_groups.cpp
	${SHARED_SRC_DIR}/wdg2/gw_sliders_pad.cpp
	${SHARED_SRC_DIR}/wdg2/gw_scrollbar.cpp
	${SHARED_SRC_DIR}/wdg2/gw_volume_slider.cpp
	${SHARED_SRC_DIR}/wdg2/scene_database.cpp


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
	${SHARED_SRC_DIR}/views/dev_select_view_setup.cpp
	${SHARED_SRC_DIR}/views/dev_select_view.cpp
	${SHARED_SRC_DIR}/views/basic_dialog.cpp
	${SHARED_SRC_DIR}/views/multi_page_dialog.cpp
	${SHARED_SRC_DIR}/views/info_dialog.cpp

	${SHARED_SRC_DIR}/unix_signal_handler.cpp
	${SHARED_SRC_DIR}/single_application.cpp
	${SHARED_SRC_DIR}/license_texts.cpp

	mwdg/mixer_gui_state.cpp
	mwdg/mixer_simple_setup.cpp

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

	${SHARED_INC_DIR}/qsnd/controls_db.hpp
	${SHARED_INC_DIR}/qsnd/controls_model.hpp
	${SHARED_INC_DIR}/qsnd/cards_model.hpp
	${SHARED_INC_DIR}/qsnd/mixer_simple_elem.hpp
	${SHARED_INC_DIR}/qsnd/mixer_simple.hpp

	${SHARED_INC_DIR}/qsnd2/controls_watcher.hpp
	${SHARED_INC_DIR}/wdg2/sliders_pad.hpp

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
	${SHARED_INC_DIR}/views/dev_select_view.hpp
	${SHARED_INC_DIR}/views/basic_dialog.hpp
	${SHARED_INC_DIR}/views/multi_page_dialog.hpp
	${SHARED_INC_DIR}/views/info_dialog.hpp

	${SHARED_INC_DIR}/unix_signal_handler.hpp
	${SHARED_INC_DIR}/single_application.hpp

	views/mixer_simple.hpp
	views/settings_dialog.hpp

	tray_mixer_icon.hpp
	tray_mixer_balloon.hpp
	tray_mixer.hpp
	main_window.hpp
	desktop_items.hpp
)


# Qt stuff
QT4_WRAP_CPP ( SRC_GUI_MOC ${SRC_GUI_HEADER_MOC} )
QT4_WRAP_CPP ( SRC_EXTRA_MOC ${SRC_EXTRA_HEADER_MOC} )


# Program
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES ( ${SHARED_INC_DIR} )
INCLUDE_DIRECTORIES ( ${ALSA_INCLUDE_DIRS} )

ADD_EXECUTABLE (
	${PROGRAM_NAME}
	${SRC_GUI}
	${SRC_GUI_MOC}
)

TARGET_LINK_LIBRARIES (
	${PROGRAM_NAME}
	${QT_LIBRARIES}
	${ALSA_LIBRARIES}
)


# Target installation

INSTALL (
	TARGETS ${PROGRAM_NAME} RUNTIME
	DESTINATION ${INSTALL_DIR_BIN} )
