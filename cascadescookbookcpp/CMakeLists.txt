project(cascadescookbookcpp)
cmake_minimum_required(VERSION 2.8)

include_directories(
  ${BB_INCLUDE_DIR}
  ${QT_CLIENT_DIR}/include
  ${QT_CLIENT_DIR}/internal
  ${QT_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/recipes
  ${CMAKE_CURRENT_LIST_DIR}/.  
)

#Find and add files to the project
BUILD_IDE_TREE(src)

#set(cascadescookbookcpp_SRCS
#  ${SOURCES}
#)

#set(cascadescookbookcpp_MOC_SRCS
# ${HEADERS}  
#)


#Find and add files to the project
#BUILD_IDE_TREE(src)

set(cascadescookbookcpp_SRCS

  src/main.cpp
  src/cascadescookbookapp.cpp  
  src/recipeitemfactory.cpp  
  src/recipeitem.cpp
  recipes/intro.cpp
  recipes/imagerecipe.cpp
  recipes/buttonrecipe.cpp
  recipes/sliderrecipe.cpp
  recipes/labelrecipe.cpp
  recipes/inputrecipe.cpp
  recipes/docklayoutrecipe.cpp
  recipes/colorrecipe.cpp
  recipes/animationrecipe.cpp
  recipes/stockcurverecipe.cpp
  recipes/nineslicerecipe.cpp
  recipes/datetimepickerrecipe.cpp
  recipes/dropdownrecipe.cpp
  recipes/activityindicatorrecipe.cpp
  recipes/progressindicatorrecipe.cpp
  recipes/orientationrecipe.cpp
  recipes/gesturehandlerrecipe.cpp
  recipes/sheetrecipe/fruititem.cpp
  recipes/sheetrecipe/fruittemfactory.cpp  
  recipes/sheetrecipe/sheetrecipe.cpp
  recipes/selectionrecipe/selection.cpp
  recipes/selectionrecipe/selectioncheckbox.cpp
  recipes/customdialogrecipe/customdialogalarm.cpp
  recipes/customdialogrecipe/customdialogrecipe.cpp
)

set(cascadescookbookcpp_MOC_SRCS
  src/cascadescookbookapp.h 
  src/recipeitemfactory.h
  src/recipeitem.h
  recipes/intro.h
  recipes/imagerecipe.h
  recipes/buttonrecipe.h
  recipes/sliderrecipe.h
  recipes/labelrecipe.h
  recipes/inputrecipe.h
  recipes/docklayoutrecipe.h
  recipes/colorrecipe.h
  recipes/colorrecipe.h
  recipes/animationrecipe.h
  recipes/stockcurverecipe.h
  recipes/nineslicerecipe.h
  recipes/datetimepickerrecipe.h
  recipes/dropdownrecipe.h
  recipes/activityindicatorrecipe.h
  recipes/progressindicatorrecipe.h
  recipes/orientationrecipe.h
  recipes/gesturehandlerrecipe.h
  recipes/sheetrecipe/fruititem.h
  recipes/sheetrecipe/fruititemfactory.h
  recipes/sheetrecipe/sheetrecipe.h
  recipes/selectionrecipe/selection.h
  recipes/selectionrecipe/selectioncheckbox.h
  recipes/customdialogrecipe/customdialogalarm.h
  recipes/customdialogrecipe/customdialogrecipe.h
)

qt4_wrap_cpp(cascadescookbookcpp_SRCS ${cascadescookbookcpp_MOC_SRCS})

# set define for the entry source file only to override the user main(argc, argv)
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS "main=cascades_user_main")

# if WIN32 add ${QT_QTMAIN_LIBRARY_RELEASE} to target_link_libraries
# MACOSX_BUNDLE
add_executable(cascadescookbookcpp
  ${cascadescookbookcpp_SRCS}
  ${cascadescookbookcpp_MOC_SRCS}
)

target_link_libraries(cascadescookbookcpp
  bbcascades
  bbcascades_main
  ${BB_LIBRARY}
  ${PLATFORM_LIBRARIES}
  )


copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/background.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/dark_background.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/white_photo.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/background.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/free_yellow_pepper.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/white_photo.png.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/Book_background.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/peach_big.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/Book_background.png.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/title_gui_buffet_empty_box.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/models/sheetmodel.xml  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/models/)


copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/animation/egg_isolated.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/animation/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/animation/egg_isolated_shadow.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/animation/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/docklayout/bean_centered.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/docklayout/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/docklayout/bean_horizontal.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/docklayout/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/docklayout/bean_vertical.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/docklayout/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/docklayout/black_page.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/docklayout/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/gesturehandler/broken.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/gesturehandler/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/gesturehandler/whole.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/gesturehandler/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/banana.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/cucumber.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/lemon.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/pear_peach.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/bananapeeled.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/docklayout.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/olives.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/peas_in_pod.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/beer.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/egg.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/orange.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/strawberries.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/broccoli.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/eggplant.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/orange_sliced.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/sugar_tile.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/button.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/lasagna.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/peach.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/sugarcandy.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/cookie.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/leek.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/pear.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/items/yellow_pepper.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/items/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/orientation/landscape.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/orientation/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/orientation/portrait.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/orientation/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/sheet/basket.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/sheet/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/sheet/fruit1.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/sheet/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/sheet/fruit2.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/sheet/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/sheet/fruit3.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/sheet/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/sheet/fruit4.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/sheet/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/sheet/fruit5.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/sheet/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/button_icon_orange_dust.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/image_button_disabled.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/button_icon_orange_green.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/image_button_enabled.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/button_icon_orange_mouldy.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/image_button_selected.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/button/button_icon_orange_ripe.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/button/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/dropdown/beer1.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/dropdown/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/dropdown/beer2.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/dropdown/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/dropdown/beer3.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/dropdown/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/dropdown/beer4.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/dropdown/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_gplay_256x256.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_scribble_light_256x256.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_gplay_256x256.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_scribble_light_256x256.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_light_toast_128x128.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_tactile_stripes_16x16.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_light_toast_128x128.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_tactile_stripes_16x16.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_nistri_16x16.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_white_stripes_16x16.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_nistri_16x16.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_white_stripes_16x16.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_pyamas_16x16.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_white_stripes_256x256.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_pyamas_16x16.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/imagepaint/Tile_white_stripes_256x256.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/imagepaint/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/nineslice/border_image_Pasta.amd  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/nineslice/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/nineslice/border_image_Pasta.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/nineslice/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/picker/banana_ancient.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/picker/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/picker/banana_new.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/picker/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/picker/banana_old.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/picker/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/picker/banana_past.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/picker/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/broken_egg.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_DoubleBounceIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuadraticIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/egg.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_DoubleBounceInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuadraticInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_BackIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_DoubleBounceOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuadraticOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_BackInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_DoubleElasticIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuarticIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_BackOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_DoubleElasticInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuarticInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_BounceIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_DoubleElasticOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuarticOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_BounceInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ElasticIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuinticIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_BounceOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ElasticInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuinticInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_CircularIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ElasticOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_QuinticOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_CircularInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ExponentialIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_SineIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_CircularOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ExponentialInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_SineInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_CubicIn.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ExponentialOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_SineOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_CubicInOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_ImplicitOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_CubicOut.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/stockcurve/icon_Linear.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/stockcurve/)

copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/customdialog/background.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/customdialog/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/customdialog/customdialog_alarm.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/customdialog/)
copy_file_to_target(cascadescookbookcpp ${CMAKE_CURRENT_LIST_DIR}/assets/images/customdialog/flame.png  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/images/customdialog/)


if(MSVC AND NOT "${LIBRARY_TYPE}" MATCHES "STATIC")
  #shared on visual studio needs dll directories in its path:
  msvc_debug_template(cascadescookbookcpp)
endif()
