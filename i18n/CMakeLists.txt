
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

# Translations files

FILE ( GLOB ts_files "ts/*.ts" )

QT4_ADD_TRANSLATION ( qm_files ${ts_files} )

ADD_CUSTOM_TARGET ( translations ALL
	DEPENDS ${qm_files} )

FOREACH( qm_src ${qm_files} )
	STRING( REGEX REPLACE ".*app_" "${L10N_PREFIX}" qm_name ${qm_src} )
	INSTALL( FILES ${qm_src}
		RENAME ${qm_name}
		DESTINATION ${INSTALL_DIR_L10N} )
ENDFOREACH( qm_src )
