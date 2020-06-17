# --- Find modules

FIND_PACKAGE (
  Qt5
  COMPONENTS
    Widgets
  REQUIRED )
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
  ${ALSA_LIBRARIES}
)


# --- Installation

INSTALL (
  TARGETS ${PROGRAM_NAME} RUNTIME
  DESTINATION ${INSTALL_DIR_BIN} )
