# Find modules
FIND_PACKAGE ( Qt5LinguistTools )

# Find translations source files
FILE ( GLOB ts_files "ts/*.ts" )

# Setup translations build target
QT5_ADD_TRANSLATION ( qm_files ${ts_files} )
ADD_CUSTOM_TARGET ( translations ALL DEPENDS ${qm_files} )

# Install translations
FOREACH ( qm_src ${qm_files} )
  STRING( REGEX REPLACE ".*app_" "${L10N_PREFIX}" qm_name ${qm_src} )
  INSTALL( FILES ${qm_src}
    RENAME ${qm_name}
    DESTINATION ${INSTALL_DIR_L10N} )
ENDFOREACH ( qm_src )
