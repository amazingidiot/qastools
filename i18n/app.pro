
HEADERS = \
	../qasconfig/src/desktop_items.hpp \
	../qasconfig/src/info_texts.hpp \
	../qasconfig/src/main_window.hpp \
	../qasconfig/src/qsnd/alsa_config_model.hpp \
	../qasconfig/src/static_tree.hpp \
	../qasconfig/src/static_tree_model.hpp \
	../qasconfig/src/views/alsa_config_view.hpp \
	../qashctl/src/desktop_items.hpp \
	../qashctl/src/info_texts.hpp \
	../qashctl/src/main_window.hpp \
	../qashctl/src/main_window_setup.hpp \
	../qashctl/src/mwdg/mixer_hctl.hpp \
	../qashctl/src/mwdg/mixer_hctl_edit_bool.hpp \
	../qashctl/src/mwdg/mixer_hctl_edit_enum.hpp \
	../qashctl/src/mwdg/mixer_hctl_edit_int.hpp \
	../qashctl/src/mwdg/mixer_hctl_edit_unsupported.hpp \
	../qashctl/src/mwdg/mixer_hctl_editor.hpp \
	../qashctl/src/mwdg/mixer_hctl_editor_data.hpp \
	../qashctl/src/mwdg/mixer_hctl_int_proxies_group.hpp \
	../qashctl/src/mwdg/mixer_hctl_int_proxy_column.hpp \
	../qashctl/src/mwdg/mixer_hctl_int_proxy_slider.hpp \
	../qashctl/src/mwdg/mixer_hctl_proxies_group.hpp \
	../qashctl/src/mwdg/mixer_hctl_proxy.hpp \
	../qashctl/src/mwdg/mixer_hctl_proxy_enum.hpp \
	../qashctl/src/mwdg/mixer_hctl_proxy_switch.hpp \
	../qashctl/src/mwdg/mixer_hctl_slider_status_widget.hpp \
	../qashctl/src/mwdg/mixer_hctl_table_model.hpp \
	../qashctl/src/mwdg/mixer_hctl_tree_model.hpp \
	../qashctl/src/views/mixer_hctl.hpp \
	../qashctl/src/views/mixer_hctl_setup.hpp \
	../qasmixer/src/cmd_options.hpp \
	../qasmixer/src/desktop_items.hpp \
	../qasmixer/src/desktop_items_setup.hpp \
	../qasmixer/src/info_texts.hpp \
	../qasmixer/src/init_globals.hpp \
	../qasmixer/src/main_window.hpp \
	../qasmixer/src/main_window_setup.hpp \
	../qasmixer/src/mwdg/mixer_gui_state.hpp \
	../qasmixer/src/mwdg/mixer_separation_info.hpp \
	../qasmixer/src/mwdg/mixer_simple_setup.hpp \
	../qasmixer/src/mwdg/mixer_sliders.hpp \
	../qasmixer/src/mwdg/mixer_sliders_proxies_column.hpp \
	../qasmixer/src/mwdg/mixer_sliders_proxies_group.hpp \
	../qasmixer/src/mwdg/mixer_sliders_proxy_slider.hpp \
	../qasmixer/src/mwdg/mixer_sliders_proxy_switch.hpp \
	../qasmixer/src/mwdg/mixer_sliders_status_widget.hpp \
	../qasmixer/src/mwdg/mixer_switches.hpp \
	../qasmixer/src/mwdg/mixer_switches_proxies_group.hpp \
	../qasmixer/src/mwdg/mixer_switches_proxy_enum.hpp \
	../qasmixer/src/mwdg/mixer_switches_proxy_switch.hpp \
	../qasmixer/src/tray_mixer.hpp \
	../qasmixer/src/tray_mixer_balloon.hpp \
	../qasmixer/src/tray_mixer_icon.hpp \
	../qasmixer/src/tray_mixer_mdev_setup.hpp \
	../qasmixer/src/tray_mixer_view_setup.hpp \
	../qasmixer/src/views/mixer_simple.hpp \
	../qasmixer/src/views/mixer_simple_setup.hpp \
	../qasmixer/src/views/settings_dialog.hpp \
	../qasmixer/src/views/settings_dialog_setup.hpp \
	../shared/src/dpe/image.hpp \
	../shared/src/dpe/image_allocator.hpp \
	../shared/src/dpe/image_request.hpp \
	../shared/src/dpe/image_set.hpp \
	../shared/src/dpe/image_set_group.hpp \
	../shared/src/dpe/image_set_meta.hpp \
	../shared/src/dpe/image_set_state.hpp \
	../shared/src/dpe/is_buffer.hpp \
	../shared/src/dpe/is_buffer_handle.hpp \
	../shared/src/dpe/paint_job.hpp \
	../shared/src/dpe/painter.hpp \
	../shared/src/dpe/painter_simple.hpp \
	../shared/src/dpe/painter_thread.hpp \
	../shared/src/dpe/painter_thread_shared.hpp \
	../shared/src/license_texts.hpp \
	../shared/src/mwdg/controls_delegate.hpp \
	../shared/src/mwdg/controls_view.hpp \
	../shared/src/mwdg/ctl_arg_view.hpp \
	../shared/src/mwdg/ctl_arg_view_card.hpp \
	../shared/src/mwdg/ctl_arg_view_integer.hpp \
	../shared/src/mwdg/ctl_arg_view_string.hpp \
	../shared/src/mwdg/event_types.hpp \
	../shared/src/mwdg/inputs_setup.hpp \
	../shared/src/mwdg/mixer_device_setup.hpp \
	../shared/src/mwdg/mixer_style.hpp \
	../shared/src/mwdg/slider_status_widget.hpp \
	../shared/src/qsnd/alsa.hpp \
	../shared/src/qsnd/alsa_config_watcher.hpp \
	../shared/src/qsnd/alsa_i18n.hpp \
	../shared/src/qsnd/card_info.hpp \
	../shared/src/qsnd/cards_model.hpp \
	../shared/src/qsnd/controls_db.hpp \
	../shared/src/qsnd/controls_model.hpp \
	../shared/src/qsnd/ctl_address.hpp \
	../shared/src/qsnd/ctl_address_arg.hpp \
	../shared/src/qsnd/ctl_format.hpp \
	../shared/src/qsnd/ctl_format_argument.hpp \
	../shared/src/qsnd/event_types.hpp \
	../shared/src/qsnd/mixer_hctl.hpp \
	../shared/src/qsnd/mixer_hctl_elem.hpp \
	../shared/src/qsnd/mixer_hctl_elem_group.hpp \
	../shared/src/qsnd/mixer_hctl_info_db.hpp \
	../shared/src/qsnd/mixer_simple.hpp \
	../shared/src/qsnd/mixer_simple_elem.hpp \
	../shared/src/qsnd/mixer_simple_filter.hpp \
	../shared/src/qsnd/mixer_simple_filter_name.hpp \
	../shared/src/qsnd/model_keys.hpp \
	../shared/src/qsnd/pcm_device_info.hpp \
	../shared/src/qsnd/pcm_subdevice_info.hpp \
	../shared/src/qsnd/pcm_subdevices_info.hpp \
	../shared/src/single_application.hpp \
	../shared/src/unix_signal_handler.hpp \
	../shared/src/views/basic_dialog.hpp \
	../shared/src/views/device_selection_view.hpp \
	../shared/src/views/device_selection_view_setup.hpp \
	../shared/src/views/info_dialog.hpp \
	../shared/src/views/message_widget.hpp \
	../shared/src/views/multi_page_dialog.hpp \
	../shared/src/views/view_base.hpp \
	../shared/src/views/view_base_setup.hpp \
	../shared/src/views/view_utility.hpp \
	../shared/src/wdg/balloon_widget.hpp \
	../shared/src/wdg/color_methods.hpp \
	../shared/src/wdg/cubic_curve.hpp \
	../shared/src/wdg/ds_imaging.hpp \
	../shared/src/wdg/ds_slider.hpp \
	../shared/src/wdg/ds_slider_meta_bg.hpp \
	../shared/src/wdg/ds_slider_painter_bevelled.hpp \
	../shared/src/wdg/ds_slider_test.hpp \
	../shared/src/wdg/ds_slider_test_dialog.hpp \
	../shared/src/wdg/ds_switch.hpp \
	../shared/src/wdg/ds_switch_painter_circle.hpp \
	../shared/src/wdg/ds_switch_painter_close.hpp \
	../shared/src/wdg/ds_switch_painter_svg.hpp \
	../shared/src/wdg/ds_widget_painter.hpp \
	../shared/src/wdg/ds_widget_style_db.hpp \
	../shared/src/wdg/ds_widget_types.hpp \
	../shared/src/wdg/equal_columns_layout.hpp \
	../shared/src/wdg/equal_columns_layout_group.hpp \
	../shared/src/wdg/event_types.hpp \
	../shared/src/wdg/fill_columns_layout.hpp \
	../shared/src/wdg/label_elide.hpp \
	../shared/src/wdg/label_width.hpp \
	../shared/src/wdg/layout_weights.hpp \
	../shared/src/wdg/pad_focus_info.hpp \
	../shared/src/wdg/pad_proxies_column.hpp \
	../shared/src/wdg/pad_proxies_group.hpp \
	../shared/src/wdg/pad_proxy.hpp \
	../shared/src/wdg/pad_proxy_enum.hpp \
	../shared/src/wdg/pad_proxy_slider.hpp \
	../shared/src/wdg/pad_proxy_style.hpp \
	../shared/src/wdg/pad_proxy_switch.hpp \
	../shared/src/wdg/pass_events.hpp \
	../shared/src/wdg/scroll_area_horizontal.hpp \
	../shared/src/wdg/scroll_area_vertical.hpp \
	../shared/src/wdg/sliders_pad.hpp \
	../shared/src/wdg/sliders_pad_data.hpp \
	../shared/src/wdg/sliders_pad_footer.hpp \
	../shared/src/wdg/sliders_pad_header.hpp \
	../shared/src/wdg/sliders_pad_header_data.hpp \
	../shared/src/wdg/sliders_pad_layout.hpp \
	../shared/src/wdg/sliders_pad_style.hpp \
	../shared/src/wdg/switches_area.hpp \
	../shared/src/wdg/switches_pad.hpp \
	../shared/src/wdg/switches_pad_widgets.hpp \
	../shared/src/wdg/switches_pad_widgets_group.hpp \
	../shared/src/wdg/text_browser.hpp \
	../shared/src/wdg/tree_view_kv.hpp \
	../shared/src/wdg/uint_mapper.hpp

SOURCES = \
	../qasconfig/src/desktop_items.cpp \
	../qasconfig/src/info_texts.cpp \
	../qasconfig/src/main.cpp \
	../qasconfig/src/main_window.cpp \
	../qasconfig/src/qsnd/alsa_config_model.cpp \
	../qasconfig/src/static_tree.cpp \
	../qasconfig/src/static_tree_model.cpp \
	../qasconfig/src/views/alsa_config_view.cpp \
	../qashctl/src/desktop_items.cpp \
	../qashctl/src/info_texts.cpp \
	../qashctl/src/main.cpp \
	../qashctl/src/main_window.cpp \
	../qashctl/src/main_window_setup.cpp \
	../qashctl/src/mwdg/mixer_hctl.cpp \
	../qashctl/src/mwdg/mixer_hctl_edit_bool.cpp \
	../qashctl/src/mwdg/mixer_hctl_edit_enum.cpp \
	../qashctl/src/mwdg/mixer_hctl_edit_int.cpp \
	../qashctl/src/mwdg/mixer_hctl_edit_unsupported.cpp \
	../qashctl/src/mwdg/mixer_hctl_editor.cpp \
	../qashctl/src/mwdg/mixer_hctl_editor_data.cpp \
	../qashctl/src/mwdg/mixer_hctl_int_proxies_group.cpp \
	../qashctl/src/mwdg/mixer_hctl_int_proxy_column.cpp \
	../qashctl/src/mwdg/mixer_hctl_int_proxy_slider.cpp \
	../qashctl/src/mwdg/mixer_hctl_proxies_group.cpp \
	../qashctl/src/mwdg/mixer_hctl_proxy.cpp \
	../qashctl/src/mwdg/mixer_hctl_proxy_enum.cpp \
	../qashctl/src/mwdg/mixer_hctl_proxy_switch.cpp \
	../qashctl/src/mwdg/mixer_hctl_slider_status_widget.cpp \
	../qashctl/src/mwdg/mixer_hctl_table_model.cpp \
	../qashctl/src/mwdg/mixer_hctl_tree_model.cpp \
	../qashctl/src/views/mixer_hctl.cpp \
	../qashctl/src/views/mixer_hctl_setup.cpp \
	../qasmixer/src/cmd_options.cpp \
	../qasmixer/src/desktop_items.cpp \
	../qasmixer/src/desktop_items_setup.cpp \
	../qasmixer/src/info_texts.cpp \
	../qasmixer/src/init_globals.cpp \
	../qasmixer/src/main.cpp \
	../qasmixer/src/main_window.cpp \
	../qasmixer/src/main_window_setup.cpp \
	../qasmixer/src/mwdg/mixer_gui_state.cpp \
	../qasmixer/src/mwdg/mixer_separation_info.cpp \
	../qasmixer/src/mwdg/mixer_simple_setup.cpp \
	../qasmixer/src/mwdg/mixer_sliders.cpp \
	../qasmixer/src/mwdg/mixer_sliders_proxies_column.cpp \
	../qasmixer/src/mwdg/mixer_sliders_proxies_group.cpp \
	../qasmixer/src/mwdg/mixer_sliders_proxy_slider.cpp \
	../qasmixer/src/mwdg/mixer_sliders_proxy_switch.cpp \
	../qasmixer/src/mwdg/mixer_sliders_status_widget.cpp \
	../qasmixer/src/mwdg/mixer_switches.cpp \
	../qasmixer/src/mwdg/mixer_switches_proxies_group.cpp \
	../qasmixer/src/mwdg/mixer_switches_proxy_enum.cpp \
	../qasmixer/src/mwdg/mixer_switches_proxy_switch.cpp \
	../qasmixer/src/tray_mixer.cpp \
	../qasmixer/src/tray_mixer_balloon.cpp \
	../qasmixer/src/tray_mixer_icon.cpp \
	../qasmixer/src/tray_mixer_mdev_setup.cpp \
	../qasmixer/src/tray_mixer_view_setup.cpp \
	../qasmixer/src/views/mixer_simple.cpp \
	../qasmixer/src/views/mixer_simple_setup.cpp \
	../qasmixer/src/views/settings_dialog.cpp \
	../qasmixer/src/views/settings_dialog_setup.cpp \
	../shared/src/dpe/image.cpp \
	../shared/src/dpe/image_allocator.cpp \
	../shared/src/dpe/image_request.cpp \
	../shared/src/dpe/image_set.cpp \
	../shared/src/dpe/image_set_group.cpp \
	../shared/src/dpe/image_set_meta.cpp \
	../shared/src/dpe/image_set_state.cpp \
	../shared/src/dpe/is_buffer.cpp \
	../shared/src/dpe/is_buffer_handle.cpp \
	../shared/src/dpe/paint_job.cpp \
	../shared/src/dpe/painter.cpp \
	../shared/src/dpe/painter_simple.cpp \
	../shared/src/dpe/painter_thread.cpp \
	../shared/src/dpe/painter_thread_shared.cpp \
	../shared/src/license_texts.cpp \
	../shared/src/mwdg/controls_delegate.cpp \
	../shared/src/mwdg/controls_view.cpp \
	../shared/src/mwdg/ctl_arg_view.cpp \
	../shared/src/mwdg/ctl_arg_view_card.cpp \
	../shared/src/mwdg/ctl_arg_view_integer.cpp \
	../shared/src/mwdg/ctl_arg_view_string.cpp \
	../shared/src/mwdg/event_types.cpp \
	../shared/src/mwdg/inputs_setup.cpp \
	../shared/src/mwdg/mixer_device_setup.cpp \
	../shared/src/mwdg/mixer_style.cpp \
	../shared/src/mwdg/slider_status_widget.cpp \
	../shared/src/qsnd/alsa.cpp \
	../shared/src/qsnd/alsa_config_watcher.cpp \
	../shared/src/qsnd/card_info.cpp \
	../shared/src/qsnd/cards_model.cpp \
	../shared/src/qsnd/controls_db.cpp \
	../shared/src/qsnd/controls_model.cpp \
	../shared/src/qsnd/ctl_address.cpp \
	../shared/src/qsnd/ctl_address_arg.cpp \
	../shared/src/qsnd/ctl_format.cpp \
	../shared/src/qsnd/ctl_format_argument.cpp \
	../shared/src/qsnd/event_types.cpp \
	../shared/src/qsnd/mixer_hctl.cpp \
	../shared/src/qsnd/mixer_hctl_elem.cpp \
	../shared/src/qsnd/mixer_hctl_elem_group.cpp \
	../shared/src/qsnd/mixer_hctl_info_db.cpp \
	../shared/src/qsnd/mixer_simple.cpp \
	../shared/src/qsnd/mixer_simple_elem.cpp \
	../shared/src/qsnd/mixer_simple_filter.cpp \
	../shared/src/qsnd/mixer_simple_filter_name.cpp \
	../shared/src/qsnd/pcm_device_info.cpp \
	../shared/src/qsnd/pcm_subdevice_info.cpp \
	../shared/src/qsnd/pcm_subdevices_info.cpp \
	../shared/src/single_application.cpp \
	../shared/src/unix_signal_handler.cpp \
	../shared/src/views/basic_dialog.cpp \
	../shared/src/views/device_selection_view.cpp \
	../shared/src/views/device_selection_view_setup.cpp \
	../shared/src/views/info_dialog.cpp \
	../shared/src/views/message_widget.cpp \
	../shared/src/views/multi_page_dialog.cpp \
	../shared/src/views/view_base.cpp \
	../shared/src/views/view_base_setup.cpp \
	../shared/src/views/view_utility.cpp \
	../shared/src/wdg/balloon_widget.cpp \
	../shared/src/wdg/color_methods.cpp \
	../shared/src/wdg/cubic_curve.cpp \
	../shared/src/wdg/ds_imaging.cpp \
	../shared/src/wdg/ds_slider.cpp \
	../shared/src/wdg/ds_slider_meta_bg.cpp \
	../shared/src/wdg/ds_slider_painter_bevelled.cpp \
	../shared/src/wdg/ds_slider_test.cpp \
	../shared/src/wdg/ds_slider_test_dialog.cpp \
	../shared/src/wdg/ds_switch.cpp \
	../shared/src/wdg/ds_switch_painter_circle.cpp \
	../shared/src/wdg/ds_switch_painter_close.cpp \
	../shared/src/wdg/ds_switch_painter_svg.cpp \
	../shared/src/wdg/ds_widget_painter.cpp \
	../shared/src/wdg/ds_widget_style_db.cpp \
	../shared/src/wdg/equal_columns_layout.cpp \
	../shared/src/wdg/equal_columns_layout_group.cpp \
	../shared/src/wdg/event_types.cpp \
	../shared/src/wdg/fill_columns_layout.cpp \
	../shared/src/wdg/label_elide.cpp \
	../shared/src/wdg/label_width.cpp \
	../shared/src/wdg/layout_weights.cpp \
	../shared/src/wdg/pad_focus_info.cpp \
	../shared/src/wdg/pad_proxies_column.cpp \
	../shared/src/wdg/pad_proxies_group.cpp \
	../shared/src/wdg/pad_proxy.cpp \
	../shared/src/wdg/pad_proxy_enum.cpp \
	../shared/src/wdg/pad_proxy_slider.cpp \
	../shared/src/wdg/pad_proxy_style.cpp \
	../shared/src/wdg/pad_proxy_switch.cpp \
	../shared/src/wdg/pass_events.cpp \
	../shared/src/wdg/scroll_area_horizontal.cpp \
	../shared/src/wdg/scroll_area_vertical.cpp \
	../shared/src/wdg/sliders_pad.cpp \
	../shared/src/wdg/sliders_pad_data.cpp \
	../shared/src/wdg/sliders_pad_footer.cpp \
	../shared/src/wdg/sliders_pad_header.cpp \
	../shared/src/wdg/sliders_pad_header_data.cpp \
	../shared/src/wdg/sliders_pad_layout.cpp \
	../shared/src/wdg/sliders_pad_style.cpp \
	../shared/src/wdg/switches_area.cpp \
	../shared/src/wdg/switches_pad.cpp \
	../shared/src/wdg/switches_pad_widgets.cpp \
	../shared/src/wdg/switches_pad_widgets_group.cpp \
	../shared/src/wdg/text_browser.cpp \
	../shared/src/wdg/tree_view_kv.cpp \
	../shared/src/wdg/uint_mapper.cpp
