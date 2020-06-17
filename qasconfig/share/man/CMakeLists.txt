# Find gzip
FIND_PACKAGE ( UnixCommands REQUIRED )

IF ( NOT GZIP )
  MESSAGE ( FATAL_ERROR "Unable to find 'gzip' program" )
ENDIF ( NOT GZIP )


# Configure manpages

SET ( man_src  ${CMAKE_CURRENT_SOURCE_DIR}/manpage.1.cmake )
SET ( man_plain ${CMAKE_CURRENT_BINARY_DIR}/manpage.1 )
SET ( man_gz ${CMAKE_CURRENT_BINARY_DIR}/manpage.1.gz )

CONFIGURE_FILE ( ${man_src} ${man_plain} )


# Compress manpages

ADD_CUSTOM_COMMAND (
  OUTPUT ${man_gz}
  COMMAND ${GZIP} -c -9 ${man_plain} > ${man_gz}
  DEPENDS ${man_plain}
  COMMENT "Building ${man_gz}" )

ADD_CUSTOM_TARGET (
  qasconfig_manpage ALL
  DEPENDS ${man_gz} )


# Installation of the manpage

INSTALL ( FILES ${man_gz}
  DESTINATION ${INSTALL_DIR_MAN}
  RENAME ${PROGRAM_NAME}.1.gz )
