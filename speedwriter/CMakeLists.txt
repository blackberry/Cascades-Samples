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

set(speedwriter_SRCS
  ${SOURCES}
)

set(speedwriter_MOC_SRCS
 ${HEADERS}  
)
 
qt4_wrap_cpp(speedwriter_SRCS ${speedwriter_MOC_SRCS})

# set define for the entry source file only to override the user main(argc, argv)
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS "main=cascades_user_main")

# if WIN32 add ${QT_QTMAIN_LIBRARY_RELEASE} to target_link_libraries
# MACOSX_BUNDLE
add_executable(speedwriter
  ${speedwriter_SRCS}
  ${speedwriter_MOC_SRCS}
)

target_link_libraries(speedwriter
  bbcascades
  bbcascades_main
    ${BB_LIBRARY}
  ${PLATFORM_LIBRARIES}
  )
 
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/speedwriter.qml ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/images/background.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/images/deep_background.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/images/dial_bg.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/images/dial_shine.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/images/needle.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(speedwriter ${CMAKE_CURRENT_LIST_DIR}/assets/images/red_needle.png ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)


if(MSVC AND NOT "${LIBRARY_TYPE}" MATCHES "STATIC")
  #shared on visual studio needs dll directories in its path:
  msvc_debug_template(speedwriter)
endif()
