find_package(PkgConfig REQUIRED)
find_package(Qt5Xml REQUIRED)
pkg_check_modules(SAILFISHSILICA sailfishsilica REQUIRED)
#pkg_check_modules(DJVULIBRE ddjvuapi REQUIRED)

include_directories(
    ${QT5XML_INCLUDE_DIR}
    ${DJVULIBRE_INCLUDE_DIR}
    )

set(djvuplugin_SRCS
    djvuviewerdjvuplugin.cpp
    djvu.cpp
    djvudocument.cpp
    djvurenderthread.cpp
    djvujob.cpp
    djvutocmodel.cpp
    djvucanvas.cpp
    #djvulinkarea.cpp
    #djvusearchmodel.cpp
    #djvuselection.cpp
    #djvuannotation.cpp
)

add_library(djvuviewerdjvuplugin MODULE ${djvuplugin_SRCS})
qt5_use_modules(djvuviewerdjvuplugin Quick Xml)

include_directories(${djvulibre_prefix})
target_link_libraries(djvuviewerdjvuplugin stdc++ ${QT_LIBRARIES} ${QT5XML_LIBRARIES} djvulibre_lib)
add_dependencies(djvuviewerdjvuplugin djvulibre_lib)

install(TARGETS djvuviewerdjvuplugin DESTINATION /usr/share/${APP_NAME}/qml/DjVu/Viewer/DJVU)
install(FILES qmldir DESTINATION /usr/share/${APP_NAME}/qml/DjVu/Viewer/DJVU)
