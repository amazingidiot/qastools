
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )


# Find modules

SET ( QT_USE_QTCORE TRUE )
SET ( QT_USE_QTGUI TRUE )
SET ( QT_USE_QTSVG TRUE )
SET ( QT_USE_QTNETWORK TRUE )

FIND_PACKAGE ( Qt4 4.6 REQUIRED )
FIND_PACKAGE ( ALSA REQUIRED )


# Configure files

INCLUDE ( CheckIncludeFiles )

CONFIGURE_FILE (
	${SHARED_SOURCE_DIR}/config.hpp.cmake
	${CMAKE_CURRENT_BINARY_DIR}/config.hpp )


# Compiler flags

ADD_DEFINITIONS ( "-Wextra -Wunused -Wall" )

SET ( CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG -fno-inline" )
SET ( CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -Wl,--as-needed" )


# Sources

SET ( SRC_GUI

	dpe/image.cpp
	dpe/image_set_meta.cpp
	dpe/image_set.cpp
	dpe/image_set_group.cpp
	dpe/image_set_state.cpp
	dpe/image_request.cpp
	dpe/image_allocator.cpp
	dpe/is_buffer_handle.cpp
	dpe/is_buffer.cpp
	dpe/paint_job.cpp
	dpe/painter.cpp
	dpe/painter_simple.cpp
	dpe/painter_thread.cpp
	dpe/painter_thread_shared.cpp

	wdg/covered_input.cpp
	wdg/covered_spinbox.cpp
	wdg/label_elide.cpp
	wdg/label_width.cpp
	wdg/text_browser.cpp
	wdg/tree_view_kv.cpp
	wdg/scroll_area_horizontal.cpp
	wdg/scroll_area_vertical.cpp
	wdg/balloon_widget.cpp

	wdg/color_methods.cpp

	wdg/ds_widget_style_db.cpp
	wdg/ds_widget_painter.cpp
	wdg/ds_imaging.cpp
	wdg/ds_switch_painter_circle.cpp
	wdg/ds_switch_painter_close.cpp
	wdg/ds_switch_painter_svg.cpp
	wdg/ds_switch.cpp

	wdg/uint_mapper.cpp
	wdg/cubic_curve.cpp
	wdg/ds_slider_meta_bg.cpp
	wdg/ds_slider_painter_bevelled.cpp
	wdg/ds_slider.cpp

	#slider_test.cpp
	#slider_test_dialog.cpp

	wdg/event_types.cpp
	wdg/pad_focus_info.cpp
	wdg/pass_events.cpp

	wdg/pad_proxy.cpp
	wdg/pad_proxy_style.cpp
	wdg/pad_proxy_slider.cpp
	wdg/pad_proxy_switch.cpp
	wdg/pad_proxy_enum.cpp
	wdg/pad_proxies_column.cpp
	wdg/pad_proxies_group.cpp

	wdg/layout_weights.cpp
	wdg/equal_columns_layout_group.cpp
	wdg/equal_columns_layout.cpp
	wdg/fill_columns_layout.cpp

	wdg/sliders_pad_layout.cpp
	wdg/sliders_pad_style.cpp
	wdg/sliders_pad_data.cpp
	wdg/sliders_pad_header_data.cpp
	wdg/sliders_pad_header.cpp
	wdg/sliders_pad_footer.cpp
	wdg/sliders_pad.cpp

	wdg/switches_pad_widgets.cpp
	wdg/switches_pad_widgets_group.cpp
	wdg/switches_pad.cpp
	wdg/switches_area.cpp

	qsnd/alsa.cpp
	qsnd/event_types.cpp
	qsnd/ctl_info.cpp
	qsnd/ctl_info_model.cpp

	qsnd/mixer_hctl_info_db.cpp
	qsnd/mixer_hctl_elem.cpp
	qsnd/mixer_hctl_elem_group.cpp
	qsnd/mixer_hctl.cpp
	qsnd/mixer_simple_elem.cpp
	qsnd/mixer_simple_filter.cpp
	qsnd/mixer_simple_filter_name.cpp
	qsnd/mixer_simple.cpp

	qsnd/ctl_def_arg.cpp
	qsnd/ctl_def.cpp
	qsnd/ctl_address_arg.cpp
	qsnd/ctl_address.cpp
	qsnd/controls_db.cpp
	qsnd/controls_model.cpp
	qsnd/cards_model.cpp
	qsnd/card_info.cpp
	qsnd/pcm_subdevice_info.cpp
	qsnd/pcm_subdevices_info.cpp
	qsnd/pcm_device_info.cpp

	mwdg/event_types.cpp
	mwdg/ctl_arg_view.cpp
	mwdg/ctl_arg_view_string.cpp
	mwdg/ctl_arg_view_integer.cpp
	mwdg/ctl_arg_view_card.cpp
	mwdg/controls_delegate.cpp
	mwdg/controls_view.cpp

	mwdg/inputs_setup.cpp
	mwdg/mixer_device_setup.cpp
	mwdg/mixer_simple_setup.cpp
	mwdg/mixer_gui_state.cpp
	mwdg/mixer_separation_info.cpp

	mwdg/slider_status_widget.cpp

	mwdg/mixer_hctl_tree_model.cpp
	mwdg/mixer_hctl_table_model.cpp
	mwdg/mixer_hctl_proxy.cpp
	mwdg/mixer_hctl_proxy_switch.cpp
	mwdg/mixer_hctl_proxy_enum.cpp
	mwdg/mixer_hctl_proxies_group.cpp
	mwdg/mixer_hctl_int_proxies_group.cpp
	mwdg/mixer_hctl_int_proxy_column.cpp
	mwdg/mixer_hctl_int_proxy_slider.cpp
	mwdg/mixer_hctl_slider_status_widget.cpp
	mwdg/mixer_hctl_editor.cpp
	mwdg/mixer_hctl_editor_data.cpp
	mwdg/mixer_hctl_edit_bool.cpp
	mwdg/mixer_hctl_edit_enum.cpp
	mwdg/mixer_hctl_edit_int.cpp
	mwdg/mixer_hctl_edit_unsupported.cpp
	mwdg/mixer_hctl.cpp

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

	tray_mixer_mdev_setup.cpp
	tray_mixer_view_setup.cpp

	mview/dev_select_view_setup.cpp
	mview/dev_select_view.cpp
	mview/info_view.cpp
	mview/settings_view_setup.cpp
	mview/settings_view.cpp

	mview/message_widget.cpp
	mview/mv_base.cpp
	mview/mv_base_setup.cpp
	mview/mv_mixer_simple.cpp
	mview/mv_mixer_simple_setup.cpp
	mview/mv_mixer_hctl.cpp
	mview/mv_mixer_hctl_setup.cpp
	mview/mv_info.cpp
	mview/mv_info_setup.cpp
	mview/switcher.cpp
	mview/switcher_setup.cpp

	tray_mixer_icon.cpp
	tray_mixer_balloon.cpp
	tray_mixer.cpp
	mixer_window_setup.cpp
	mixer_window.cpp
	cmd_options.cpp
	desktop_items_setup.cpp
	desktop_items.cpp

	unix_signal_handler.cpp
	single_application.cpp
	init_globals.cpp
	info_texts.cpp
	main.cpp
)


SET ( SRC_GUI_HEADER_MOC

	dpe/is_buffer.hpp
	dpe/image_allocator.hpp

	wdg/covered_input.hpp
	wdg/covered_spinbox.hpp
	wdg/tree_view_kv.hpp
	wdg/ds_slider.hpp
	wdg/ds_switch.hpp
	wdg/balloon_widget.hpp

	#slider_test.hpp
	#slider_test_dialog.hpp

	wdg/pad_proxy.hpp
	wdg/pad_proxy_slider.hpp
	wdg/pad_proxy_switch.hpp
	wdg/pad_proxy_enum.hpp
	wdg/pad_proxies_column.hpp
	wdg/pad_proxies_group.hpp

	wdg/sliders_pad_data.hpp
	wdg/sliders_pad_header.hpp
	wdg/sliders_pad_footer.hpp
	wdg/sliders_pad.hpp
	wdg/switches_pad.hpp

	qsnd/controls_db.hpp
	qsnd/controls_model.hpp
	qsnd/cards_model.hpp
	qsnd/ctl_info.hpp
	qsnd/ctl_info_model.hpp
	qsnd/mixer_hctl_elem.hpp
	qsnd/mixer_hctl.hpp
	qsnd/mixer_simple_elem.hpp
	qsnd/mixer_simple.hpp

	mwdg/controls_delegate.hpp
	mwdg/controls_view.hpp
	mwdg/ctl_arg_view.hpp
	mwdg/ctl_arg_view_string.hpp
	mwdg/ctl_arg_view_integer.hpp
	mwdg/ctl_arg_view_card.hpp
	mwdg/slider_status_widget.hpp

	mwdg/mixer_hctl_tree_model.hpp
	mwdg/mixer_hctl_table_model.hpp
	mwdg/mixer_hctl_proxy.hpp
	mwdg/mixer_hctl_proxy_switch.hpp
	mwdg/mixer_hctl_proxy_enum.hpp
	mwdg/mixer_hctl_proxies_group.hpp
	mwdg/mixer_hctl_int_proxies_group.hpp
	mwdg/mixer_hctl_int_proxy_column.hpp
	mwdg/mixer_hctl_int_proxy_slider.hpp
	mwdg/mixer_hctl_slider_status_widget.hpp
	mwdg/mixer_hctl_editor.hpp
	mwdg/mixer_hctl_edit_bool.hpp
	mwdg/mixer_hctl_edit_enum.hpp
	mwdg/mixer_hctl_edit_int.hpp
	mwdg/mixer_hctl_edit_unsupported.hpp
	mwdg/mixer_hctl.hpp

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

	single_application.hpp

	mview/dev_select_view.hpp
	mview/info_view.hpp
	mview/settings_view.hpp

	mview/message_widget.hpp
	mview/mv_base.hpp
	mview/mv_mixer_simple.hpp
	mview/mv_mixer_hctl.hpp
	mview/mv_info.hpp
	mview/switcher.hpp

	tray_mixer_icon.hpp
	tray_mixer_balloon.hpp
	tray_mixer.hpp
	mixer_window.hpp
	desktop_items.hpp

	unix_signal_handler.hpp
)


# Qt stuff

INCLUDE ( ${QT_USE_FILE} )

QT4_WRAP_CPP ( SRC_GUI_MOC ${SRC_GUI_HEADER_MOC} )
QT4_WRAP_CPP ( SRC_EXTRA_MOC ${SRC_EXTRA_HEADER_MOC} )


# Program

INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES ( ${SHARED_INCLUDE_DIR} )
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
