project(kakel)
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


set(kakel_SRCS
  ${SOURCES}
)

set(kakel_MOC_SRCS
 ${HEADERS}  
)

qt4_wrap_cpp(kakel_SRCS ${kakel_MOC_SRCS})

# set define for the entry source file only to override the user main(argc, argv)
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS "main=cascades_user_main")

# if WIN32 add ${QT_QTMAIN_LIBRARY_RELEASE} to target_link_libraries
# MACOSX_BUNDLE
add_executable(kakel
  ${kakel_SRCS}
  ${kakel_MOC_SRCS}
)

target_link_libraries(kakel
  bbcascades
  bbcascades_main
  ${BB_LIBRARY}
  ${PLATFORM_LIBRARIES}  
)

copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/main.qml ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/background.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/icon_complex.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/icon_simple.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/tile1.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/tile2.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/tile3.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/tile4.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/tile5.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/tile6.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(kakel ${CMAKE_CURRENT_LIST_DIR}/assets/images/empty.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)


if(MSVC AND NOT "${LIBRARY_TYPE}" MATCHES "STATIC")
  #shared on visual studio needs dll directories in its path:
  msvc_debug_template(kakel)
endif()
