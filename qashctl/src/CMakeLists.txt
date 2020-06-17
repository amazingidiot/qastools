# --- Find modules

FIND_PACKAGE (
  Qt5
  COMPONENTS
    Widgets
    Svg
  REQUIRED )
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
  ${SHARED_SRC_DIR}/qsnd/alsa_config_watcher.cpp
  ${SHARED_SRC_DIR}/qsnd/event_types.cpp
  ${SHARED_SRC_DIR}/qsnd/ctl_address_argument.cpp
  ${SHARED_SRC_DIR}/qsnd/ctl_address.cpp
  ${SHARED_SRC_DIR}/qsnd/ctl_format_argument.cpp
  ${SHARED_SRC_DIR}/qsnd/ctl_format.cpp
  ${SHARED_SRC_DIR}/qsnd/card_info.cpp
  ${SHARED_SRC_DIR}/qsnd/cards_model.cpp
  ${SHARED_SRC_DIR}/qsnd/controls_database.cpp
  ${SHARED_SRC_DIR}/qsnd/controls_model.cpp
  ${SHARED_SRC_DIR}/qsnd/mixer_hctl.cpp
  ${SHARED_SRC_DIR}/qsnd/mixer_hctl_elem.cpp
  ${SHARED_SRC_DIR}/qsnd/mixer_hctl_elem_group.cpp
  ${SHARED_SRC_DIR}/qsnd/mixer_hctl_info_db.cpp
  ${SHARED_SRC_DIR}/qsnd/udev_device_lookout.cpp

  ${SHARED_SRC_DIR}/dpe/image_allocator.cpp
  ${SHARED_SRC_DIR}/dpe/image.cpp
  ${SHARED_SRC_DIR}/dpe/image_request.cpp
  ${SHARED_SRC_DIR}/dpe/image_set.cpp
  ${SHARED_SRC_DIR}/dpe/image_set_group.cpp
  ${SHARED_SRC_DIR}/dpe/image_set_meta.cpp
  ${SHARED_SRC_DIR}/dpe/image_set_state.cpp
  ${SHARED_SRC_DIR}/dpe/is_buffer.cpp
  ${SHARED_SRC_DIR}/dpe/is_buffer_handle.cpp
  ${SHARED_SRC_DIR}/dpe/painter.cpp
  ${SHARED_SRC_DIR}/dpe/painter_simple.cpp
  ${SHARED_SRC_DIR}/dpe/painter_thread.cpp
  ${SHARED_SRC_DIR}/dpe/painter_thread_shared.cpp
  ${SHARED_SRC_DIR}/dpe/paint_job.cpp

  ${SHARED_SRC_DIR}/wdg/label_width.cpp
  ${SHARED_SRC_DIR}/wdg/label_elide.cpp
  ${SHARED_SRC_DIR}/wdg/tree_view_kv.cpp
  ${SHARED_SRC_DIR}/wdg/text_browser.cpp
  ${SHARED_SRC_DIR}/wdg/scroll_area_horizontal.cpp
  ${SHARED_SRC_DIR}/wdg/scroll_area_vertical.cpp
  ${SHARED_SRC_DIR}/wdg/color_methods.cpp
  ${SHARED_SRC_DIR}/wdg/cubic_curve.cpp
  ${SHARED_SRC_DIR}/wdg/uint_mapper.cpp
  ${SHARED_SRC_DIR}/wdg/ds_widget_painter.cpp
  ${SHARED_SRC_DIR}/wdg/ds_widget_style_db.cpp
  ${SHARED_SRC_DIR}/wdg/ds_imaging.cpp
  ${SHARED_SRC_DIR}/wdg/ds_switch.cpp
  ${SHARED_SRC_DIR}/wdg/ds_switch_painter_circle.cpp
  ${SHARED_SRC_DIR}/wdg/ds_switch_painter_svg.cpp
  ${SHARED_SRC_DIR}/wdg/ds_slider.cpp
  ${SHARED_SRC_DIR}/wdg/ds_slider_meta_bg.cpp
  ${SHARED_SRC_DIR}/wdg/ds_slider_painter_bevelled.cpp
  ${SHARED_SRC_DIR}/wdg/event_types.cpp
  ${SHARED_SRC_DIR}/wdg/pass_events.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxies_column.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxies_group.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxy.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxy_enum.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxy_slider.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxy_style.cpp
  ${SHARED_SRC_DIR}/wdg/pad_proxy_switch.cpp
  ${SHARED_SRC_DIR}/wdg/pad_focus_info.cpp
  ${SHARED_SRC_DIR}/wdg/layout_weights.cpp
  ${SHARED_SRC_DIR}/wdg/equal_columns_layout_group.cpp
  ${SHARED_SRC_DIR}/wdg/equal_columns_layout.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad_data.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad_footer.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad_header.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad_header_data.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad_layout.cpp
  ${SHARED_SRC_DIR}/wdg/sliders_pad_style.cpp

  ${SHARED_SRC_DIR}/mwdg/event_types.cpp
  ${SHARED_SRC_DIR}/mwdg/slider_status_widget.cpp
  ${SHARED_SRC_DIR}/mwdg/controls_view.cpp
  ${SHARED_SRC_DIR}/mwdg/controls_delegate.cpp
  ${SHARED_SRC_DIR}/mwdg/ctl_arg_view.cpp
  ${SHARED_SRC_DIR}/mwdg/ctl_arg_view_integer.cpp
  ${SHARED_SRC_DIR}/mwdg/ctl_arg_view_string.cpp
  ${SHARED_SRC_DIR}/mwdg/ctl_arg_view_card.cpp
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

  ${SHARED_SRC_DIR}/license_texts.cpp

  mwdg/mixer_hctl.cpp
  mwdg/mixer_hctl_edit_bool.cpp
  mwdg/mixer_hctl_edit_enum.cpp
  mwdg/mixer_hctl_edit_int.cpp
  mwdg/mixer_hctl_editor.cpp
  mwdg/mixer_hctl_editor_data.cpp
  mwdg/mixer_hctl_edit_unsupported.cpp
  mwdg/mixer_hctl_int_proxies_group.cpp
  mwdg/mixer_hctl_int_proxy_column.cpp
  mwdg/mixer_hctl_int_proxy_slider.cpp
  mwdg/mixer_hctl_proxies_group.cpp
  mwdg/mixer_hctl_proxy.cpp
  mwdg/mixer_hctl_proxy_enum.cpp
  mwdg/mixer_hctl_proxy_switch.cpp
  mwdg/mixer_hctl_slider_status_widget.cpp
  mwdg/mixer_hctl_table_model.cpp
  mwdg/mixer_hctl_tree_model.cpp

  views/mixer_hctl_setup.cpp
  views/mixer_hctl.cpp

  info_texts.cpp
  main_window_setup.cpp
  main_window.cpp
  desktop_items.cpp
  main.cpp
)


# --- Compiler flags

SET ( CMAKE_CXX_STANDARD 17 )
SET ( CMAKE_CXX_STANDARD_REQUIRED OFF )
SET ( CMAKE_CXX_EXTENSIONS OFF )
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wunused -Wall" )
SET ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fno-inline" )
SET ( CMAKE_CXX_FLAGS_RELEASE "-O2 ${CMAKE_CXX_FLAGS_RELEASE}" )


# --- Include directories

INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES ( ${SHARED_INC_DIR} )
INCLUDE_DIRECTORIES ( ${ALSA_INCLUDE_DIRS} )


# --- Executable

ADD_EXECUTABLE ( ${PROGRAM_NAME}
  ${SRC_GUI}
  ${SRC_GUI_MOC}
)
SET_TARGET_PROPERTIES ( ${PROGRAM_NAME} PROPERTIES AUTOMOC ON )
TARGET_LINK_LIBRARIES ( ${PROGRAM_NAME}
  Qt5::Widgets
  Qt5::Svg
  ${ALSA_LIBRARIES}
  ${UDEV_LIBRARIES}
)


# --- Installation

INSTALL (
  TARGETS ${PROGRAM_NAME} RUNTIME
  DESTINATION ${INSTALL_DIR_BIN} )
