project(djvu-viewer)

cmake_minimum_required(VERSION 2.8.12)

include(GNUInstallDirs)
include(ExternalProject)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#list(APPEND QML_IMPORT_PATH "/usr/share/djvu-view/qml")
#list(REMOVE_DUPLICATES QML_IMPORT_PATH)
#set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
#    CACHE STRING "extra qml import paths"
#    FORCE
#)
#set(QML_IMPORT_PATH /usr/share/djvu-view/qml CACHE STRING "" FORCE) 
#set(QT_QML_OUTPUT_DIRECTORY /usr/share/djvu-viewer)
#cmake_policy(SET CMP0002 OLD)

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(QtSparql REQUIRED)
find_package(Booster REQUIRED)

include(cmake/QtTranslationWithID.cmake)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES}
    ${BOOSTER_INCLUDE_DIR}
    ${QTSPARQL_INCLUDE_DIR}
)

# paths
set(AURORAOS_SHARED_LIBRARY_DIR /usr/share/djvu-viewer/lib/)


# libdjvu
set(djvulibre_prefix ${CMAKE_HOME_DIRECTORY}/dependencies/djvulibre)
set(djvulibre_configure ${djvulibre_prefix}/autogen.sh)
set(djvulibre_build make -j4)
set(djvulibre_lib_dir ${djvulibre_prefix}/libdjvu/.libs)

#set(djvulibre_install make install)

ExternalProject_Add(djvulibre
   SOURCE_DIR ${djvulibre_prefix}
   BUILD_IN_SOURCE 1
   CONFIGURE_COMMAND ${djvulibre_configure}
   BUILD_COMMAND ${djvulibre_build}
   INSTALL_COMMAND ""
   )

include_directories(${djvulibre_prefix})
add_library(djvulibre_lib STATIC IMPORTED)
add_dependencies(djvulibre_lib djvulibre)
set_target_properties(djvulibre_lib PROPERTIES IMPORTED_LOCATION ${djvulibre_lib_dir}/libdjvulibre.so)
install(FILES
    ${djvulibre_lib_dir}/libdjvulibre.so
    ${djvulibre_lib_dir}/libdjvulibre.so.21
    ${djvulibre_lib_dir}/libdjvulibre.so.21.7.0
    DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR})
# end of libdjvu

#set(CALLIGRA_QML_PLUGIN_DIR ${CMAKE_INSTALL_LIBDIR}/calligra CACHE PATH "The location of the Calligra QtQuick Components")
configure_file(config.h.in config.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fPIC -pie -rdynamic -Wall")

add_subdirectory(plugin)
add_subdirectory(djvu)

set(djvuviewer_SRCS
    main.cpp
    dbusadaptor.cpp
    models/filtermodel.cpp
    models/documentlistmodel.cpp
    models/documentprovider.cpp
    models/trackerdocumentprovider.cpp
)

set(djvuviewer_QML_SRCS
    qml/CoverPage.qml
    qml/FileListPage.qml
    qml/Main.qml
    qml/CoverFileItem.qml
    qml/SortTypeSelectionPage.qml
)

file(GLOB plugin_QML_SRCS plugin/*.qml)

set(djvuviewer_TS_SRCS
    ${djvuviewer_SRCS}
    ${djvuviewer_QML_SRCS}
    ${plugin_QML_SRCS}
    qml/translations/StoreDescription.qml
)

create_translation(engen_qm_file ${CMAKE_BINARY_DIR}/djvu-viewer.ts ${djvuviewer_TS_SRCS})

add_executable(djvu-viewer ${djvuviewer_SRCS} ${engen_qm_file})
qt5_use_modules(djvu-viewer Widgets Quick DBus)
target_link_libraries(djvu-viewer stdc++ ${QT_LIBRARIES} ${BOOSTER_LIBRARY} ${QTSPARQL_LIBRARY})

install(TARGETS djvu-viewer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES
    djvu-viewer.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES ${djvuviewer_QML_SRCS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/djvu-viewer/qml
)
install(FILES ${engen_qm_file} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/translations)
#install(FILES ${CMAKE_BINARY_DIR}/djvu-viewer.ts DESTINATION ${CMAKE_INSTALL_PREFIX}/share/translations/source)
install(FILES djvu-viewer.privileges DESTINATION ${CMAKE_INSTALL_PREFIX}/share/mapplauncherd/privileges.d)

install(FILES icons/86x86/djvu-viewer.png
DESTINATION share/icons/hicolor/86x86/apps)

install(FILES icons/108x108/djvu-viewer.png
DESTINATION share/icons/hicolor/108x108/apps)

install(FILES icons/128x128/djvu-viewer.png
DESTINATION share/icons/hicolor/128x128/apps)

#install(FILES icons/256x256/djvu-viewer.png
#DESTINATION share/icons/hicolor/256x256/apps)

install(FILES icons/172x172/djvu-viewer.png
DESTINATION share/icons/hicolor/172x172/apps)

