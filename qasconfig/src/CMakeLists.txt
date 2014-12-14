
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8.11 )


# --- Find modules

if ( USE_QT5 )
FIND_PACKAGE ( Qt5Widgets )
else()
SET ( QT_USE_QTCORE TRUE )
SET ( QT_USE_QTGUI TRUE )
FIND_PACKAGE ( Qt4 4.6 REQUIRED )
endif()
FIND_PACKAGE ( ALSA REQUIRED )


# --- Configure files

INCLUDE ( CheckIncludeFiles )

CONFIGURE_FILE (
	${SHARED_SRC_DIR}/qastools_config.hpp.cmake
	${CMAKE_CURRENT_BINARY_DIR}/qastools_config.hpp )


# --- Sources

SET ( SRC_GUI
	${SHARED_SRC_DIR}/qsnd/alsa.cpp
	${SHARED_SRC_DIR}/qsnd/alsa_config_watcher.cpp
	${SHARED_SRC_DIR}/wdg/scroll_area_vertical.cpp
	${SHARED_SRC_DIR}/wdg/text_browser.cpp
	${SHARED_SRC_DIR}/wdg/tree_view_kv.cpp
	${SHARED_SRC_DIR}/views/view_utility.cpp
	${SHARED_SRC_DIR}/views/basic_dialog.cpp
	${SHARED_SRC_DIR}/views/multi_page_dialog.cpp
	${SHARED_SRC_DIR}/views/info_dialog.cpp
	${SHARED_SRC_DIR}/license_texts.cpp

	qsnd/alsa_config_model.cpp
	views/alsa_config_view.cpp

	info_texts.cpp
	static_tree.cpp
	static_tree_model.cpp
	main_window.cpp
	desktop_items.cpp
	main.cpp
)

SET ( SRC_GUI_HEADER_MOC
	${SHARED_INC_DIR}/qsnd/alsa_config_watcher.hpp
	${SHARED_INC_DIR}/wdg/tree_view_kv.hpp
	${SHARED_INC_DIR}/views/basic_dialog.hpp
	${SHARED_INC_DIR}/views/multi_page_dialog.hpp
	${SHARED_INC_DIR}/views/info_dialog.hpp

	qsnd/alsa_config_model.hpp
	views/alsa_config_view.hpp

	main_window.hpp
)


# --- Compiler flags

SET ( CXX_COMMON_FLAGS "-fPIC -fPIE -Wextra -Wunused -Wall" )
SET ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-inline  ${CXX_COMMON_FLAGS}" )
SET ( CMAKE_CXX_FLAGS_RELEASE "-O2 ${CXX_COMMON_FLAGS} -Wl,--as-needed" )


# --- Include directories

INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES ( ${SHARED_INC_DIR} )
INCLUDE_DIRECTORIES ( ${ALSA_INCLUDE_DIRS} )


# --- QT extras

if ( USE_QT5 )
QT5_WRAP_CPP ( SRC_GUI_MOC ${SRC_GUI_HEADER_MOC} )
QT5_WRAP_CPP ( SRC_EXTRA_MOC ${SRC_EXTRA_HEADER_MOC} )
else()
INCLUDE ( ${QT_USE_FILE} )

QT4_WRAP_CPP ( SRC_GUI_MOC ${SRC_GUI_HEADER_MOC} )
QT4_WRAP_CPP ( SRC_EXTRA_MOC ${SRC_EXTRA_HEADER_MOC} )
endif()


# --- Executable

ADD_EXECUTABLE ( ${PROGRAM_NAME}
	${SRC_GUI}
	${SRC_GUI_MOC}
)

if ( USE_QT5 )
TARGET_LINK_LIBRARIES ( ${PROGRAM_NAME}
	Qt5::Widgets
	${ALSA_LIBRARIES}
)
else()
TARGET_LINK_LIBRARIES ( ${PROGRAM_NAME}
	${QT_LIBRARIES}
	${ALSA_LIBRARIES}
)
endif()


# --- Installation

INSTALL (
	TARGETS ${PROGRAM_NAME} RUNTIME
	DESTINATION ${INSTALL_DIR_BIN} )
