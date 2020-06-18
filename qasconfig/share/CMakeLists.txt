# Desktop file

SET ( desktop_file_in "${CMAKE_CURRENT_SOURCE_DIR}/application.desktop" )
SET ( desktop_file    "${CMAKE_CURRENT_BINARY_DIR}/${PROGRAM_NAME}.desktop" )

CONFIGURE_FILE ( ${desktop_file_in} ${desktop_file} )

INSTALL( FILES ${desktop_file} DESTINATION ${INSTALL_DIR_DESKTOP} )

# AppStream meta data

SET ( appstream_file_in "${CMAKE_CURRENT_SOURCE_DIR}/application.appdata.xml" )
SET ( appstream_file    "${CMAKE_CURRENT_BINARY_DIR}/${PROGRAM_NAME}.appdata.xml" )

CONFIGURE_FILE ( ${appstream_file_in} ${appstream_file} )

INSTALL ( FILES ${appstream_file} DESTINATION ${INSTALL_DIR_APPSTREAM} )

# Subdirectories

ADD_SUBDIRECTORY ( man )
