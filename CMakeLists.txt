CMAKE_MINIMUM_REQUIRED ( VERSION 3.6 )
PROJECT ( QasTools
    VERSION 0.23.0
    LANGUAGES CXX )


# Build type

SET ( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build type" )
SET ( BUILD_LIST "qascommon,qasconfig,qashctl,qasmixer" CACHE STRING
      "List of parts to build" )


# program/package/version suffix

SET ( PROGRAM_SUFFIX "" CACHE STRING "Program suffix" )
SET ( VERSION_SUFFIX "" CACHE STRING "Version suffix" )
SET ( PACKAGE_SUFFIX "" CACHE STRING "Package suffix" )


# Package name

SET ( PACKAGE_TITLE "QasTools" CACHE STRING "Package title" )
SET ( PACKAGE_NAME "qastools${PACKAGE_SUFFIX}" CACHE STRING "Package name" )


# Package version

SET ( PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}" )
SET ( PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
SET ( PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

SET ( PACKAGE_VERSION
    "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}${VERSION_SUFFIX}" )


# Installation directories

SET ( INSTALL_DIR_BIN "bin" CACHE STRING
      "Installation directory for binaries" )
SET ( INSTALL_DIR_MAN "share/man/man1" CACHE STRING
      "Installation directory for manpages" )
SET ( INSTALL_DIR_APPSTREAM "share/metainfo" CACHE STRING
      "Installation directory for AppStream metadata" )
SET ( INSTALL_DIR_DESKTOP "share/applications" CACHE STRING
      "Installation directory for desktop files" )

SET ( INSTALL_DIR_DATA "share/${PACKAGE_NAME}" CACHE STRING
      "Installation directory for application data" )
SET ( INSTALL_DIR_L10N "${INSTALL_DIR_DATA}/l10n" CACHE STRING
      "Installation directory for localization files" )
SET ( INSTALL_DIR_APP_ICONS "${INSTALL_DIR_DATA}/icons" CACHE STRING
      "Installation directory for application icons" )
SET ( INSTALL_DIR_WIDGETS_GRAPHICS "${INSTALL_DIR_DATA}/widgets" CACHE STRING
      "Installation directory for widget graphics" )


# Icon installation directories

SET ( INSTALL_DIR_ICONS_PNG_16 "share/icons/hicolor/16x16/apps" CACHE STRING
      "Installation directory for application icons (16x16)" )
SET ( INSTALL_DIR_ICONS_PNG_32 "share/icons/hicolor/32x32/apps" CACHE STRING
      "Installation directory for application icons (32x32)" )
SET ( INSTALL_DIR_ICONS_PNG_48 "share/icons/hicolor/48x48/apps" CACHE STRING
      "Installation directory for application icons (48x48)" )
SET ( INSTALL_DIR_ICONS_PNG_64 "share/icons/hicolor/64x64/apps" CACHE STRING
      "Installation directory for application icons (64x64)" )
SET ( INSTALL_DIR_ICONS_SVG "share/icons/hicolor/scalable/apps" CACHE STRING
      "Installation directory for application icons (scalable)" )


# Localization file prefix

SET ( L10N_PREFIX "${PACKAGE_NAME}_" )


# Shared resources

SET ( SHARED_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shared" )
SET ( SHARED_SRC_DIR "${SHARED_RES_DIR}/src" )
SET ( SHARED_INC_DIR "${SHARED_RES_DIR}/src" )


# Installation

IF ( NOT SKIP_LICENSE_INSTALL )
  INSTALL( FILES "COPYING"
    DESTINATION ${INSTALL_DIR_DATA} )
ENDIF ( NOT SKIP_LICENSE_INSTALL )


# Process subdirectories

IF ( ${BUILD_LIST} MATCHES "qascommon" )
  ADD_SUBDIRECTORY ( i18n )
  ADD_SUBDIRECTORY ( shared )
ENDIF ( ${BUILD_LIST} MATCHES "qascommon" )

IF ( ${BUILD_LIST} MATCHES "qasconfig" )
  ADD_SUBDIRECTORY ( qasconfig )
ENDIF ( ${BUILD_LIST} MATCHES "qasconfig" )

IF ( ${BUILD_LIST} MATCHES "qashctl" )
  ADD_SUBDIRECTORY ( qashctl )
ENDIF ( ${BUILD_LIST} MATCHES "qashctl" )

IF ( ${BUILD_LIST} MATCHES "qasmixer" )
  ADD_SUBDIRECTORY ( qasmixer )
ENDIF ( ${BUILD_LIST} MATCHES "qasmixer" )
