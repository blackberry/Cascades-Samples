
project(starshipsettings)
cmake_minimum_required(VERSION 2.8)

include_directories(
  ${BB_INCLUDE_DIR}
  ${QT_CLIENT_DIR}/include
  ${QT_CLIENT_DIR}/internal
  ${QT_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
)

#Find and add files to the project
BUILD_IDE_TREE(src)


set(starshipsettings_SRCS
  ${SOURCES}
)

set(starshipsettings_MOC_SRCS
 ${HEADERS}  
)

qt4_wrap_cpp(starshipsettings_SRCS ${starshipsettings_MOC_SRCS})

# set define for the entry source file only to override the user main(argc, argv)
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS "main=cascades_user_main")

# if WIN32 add ${QT_QTMAIN_LIBRARY_RELEASE} to target_link_libraries
# MACOSX_BUNDLE
add_executable(starshipsettings
  ${starshipsettings_SRCS}
  ${starshipsettings_MOC_SRCS}
)

target_link_libraries(starshipsettings
  bbcascades
  bbcascades_main
  ${BB_LIBRARY}
  ${PLATFORM_LIBRARIES}  
)

copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/CheckBoxWithText.qml  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/WarpDrive.qml  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/main.qml  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)


copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/images/Background.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/images/Warp_Drive.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/images/Warp_Drive_Energy.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(starshipsettings ${CMAKE_CURRENT_LIST_DIR}/assets/images/_Tooltip.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)

if(MSVC AND NOT "${LIBRARY_TYPE}" MATCHES "STATIC")
  #shared on visual studio needs dll directories in its path:
  msvc_debug_template(starshipsettings)
endif()
