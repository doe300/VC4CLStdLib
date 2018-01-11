cmake_minimum_required (VERSION 3.1)

####
# General configuration
####
# Option whether to create deb package
option(BUILD_DEB_PACKAGE "Enables creating .deb package" ON)


project(VC4CLStdLib VERSION 0.4)


#Include headers in the project structure
file( GLOB HDRS "${PROJECT_SOURCE_DIR}/include/*.h")
add_library(VC4CLStdLib STATIC ${HDRS})
set_target_properties(VC4CLStdLib PROPERTIES LINKER_LANGUAGE C)

##
# Installation targets
##
# Adds the public headers to the target, so they are exported
target_include_directories(VC4CLStdLib PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>  $<INSTALL_INTERFACE:include/vc4cl-stdlib>)
# Creates the install target for the headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include/vc4cl-stdlib FILES_MATCHING PATTERN "*.h")
# Adds custom uninstall command
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")

####
# Building package
####
if (BUILD_DEB_PACKAGE)
	message(STATUS "build deb package...")

	set(CPACK_GENERATOR "DEB")
	set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
	set(CPACK_PACKAGE_NAME "vc4cl-stdlib")
	string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d")
	set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}-${BUILD_TIMESTAMP}")
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
	set(CPACK_PACKAGE_VENDOR "doe300")
	set(CPACK_PACKAGE_CONTACT "doe300@example.com")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenCL C headers for the VC4CL implementation (rasberrypi only)")
	set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/doe300/VC4CLStdLib")
	include(CPack)
endif (BUILD_DEB_PACKAGE)
