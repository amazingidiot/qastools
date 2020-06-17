# Install widgets graphics

FILE ( GLOB switch_icons "widgets/sw_joined_bg_*.svg" )
FOREACH ( switch_icon ${switch_icons} )
  INSTALL ( FILES ${switch_icon}
    DESTINATION ${INSTALL_DIR_WIDGETS_GRAPHICS} )
ENDFOREACH( switch_icon )


FILE ( GLOB switch_icons "widgets/sw_joined_handle_*.svg" )
FOREACH ( switch_icon ${switch_icons} )
  INSTALL ( FILES ${switch_icon}
    DESTINATION ${INSTALL_DIR_WIDGETS_GRAPHICS} )
ENDFOREACH( switch_icon )

