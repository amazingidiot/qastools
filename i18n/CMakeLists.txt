
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8.11 )

# Find modules
if ( USE_QT5 )
FIND_PACKAGE ( Qt5LinguistTools )
else()
SET ( QT_USE_QTCORE TRUE )
FIND_PACKAGE ( Qt4 4.6 REQUIRED )
INCLUDE ( ${QT_USE_FILE} )
endif()

# Find translations source files
FILE ( GLOB ts_files "ts/*.ts" )

# Setup translations build target
if ( USE_QT5 )
QT5_ADD_TRANSLATION ( qm_files ${ts_files} )
else()
QT4_ADD_TRANSLATION ( qm_files ${ts_files} )
endif()
ADD_CUSTOM_TARGET ( translations ALL DEPENDS ${qm_files} )

# Install translations
FOREACH ( qm_src ${qm_files} )
	STRING( REGEX REPLACE ".*app_" "${L10N_PREFIX}" qm_name ${qm_src} )
	INSTALL( FILES ${qm_src}
		RENAME ${qm_name}
		DESTINATION ${INSTALL_DIR_L10N} )
ENDFOREACH ( qm_src )
