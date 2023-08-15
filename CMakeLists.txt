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
set(APP_NAME info.you-ra.djvuviewer)
#cmake_policy(SET CMP0002 OLD)

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Booster REQUIRED)

include(cmake/QtTranslationWithID.cmake)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES}
    ${BOOSTER_INCLUDE_DIR}
)

# paths
set(AURORAOS_SHARED_LIBRARY_DIR /usr/share/${APP_NAME}/lib/)

# libjpeg-turbo
set(turbojpeg_lib_dir ${CMAKE_BINARY_DIR}/dependencies/libjpeg)
set(ENABLE_STATIC FALSE CACHE INTERNAL "disable static libjpeg")
set(WITH_TURBOJPEG FALSE CACHE INTERNAL "without turbojpeg extension")

#set(CMAKE_SKIP_INSTALL_RULES TRUE)
file(COPY ${CMAKE_HOME_DIRECTORY}/dependencies/djvulibre
    DESTINATION ${CMAKE_BINARY_DIR}/dependencies)

ExternalProject_Add(libjpeg
   SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/dependencies/libjpeg
   BINARY_DIR ${CMAKE_BINARY_DIR}/dependencies/libjpeg
   BUILD_IN_SOURCE 0
   CONFIGURE_COMMAND cmake -DENABLE_STATIC=FALSE -DWITH_TURBOJPEG=FALSE .
   BUILD_COMMAND make -j4
   INSTALL_COMMAND ""
   )


#add_subdirectory("dependencies/libjpeg" ) EXCLUDE_FROM_ALL)
# end of libjpeg-turbo

# libdjvu
#add_subdirectory("dependencies/djvulibre")

set(djvulibre_prefix ${CMAKE_BINARY_DIR}/dependencies/djvulibre)
set(djvulibre_configure ${djvulibre_prefix}/autogen.sh)
set(djvulibre_build make -j4)
set(djvulibre_lib_dir ${djvulibre_prefix}/libdjvu/.libs)

#set(djvulibre_install make install)
file(COPY ${CMAKE_HOME_DIRECTORY}/dependencies/djvulibre
    DESTINATION ${CMAKE_BINARY_DIR}/dependencies)

ExternalProject_Add(djvulibre
   SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/dependencies/djvulibre
   BINARY_DIR ${djvulibre_prefix}
   BUILD_IN_SOURCE 0
   CONFIGURE_COMMAND ${djvulibre_configure}
   BUILD_COMMAND ${djvulibre_build}
   INSTALL_COMMAND ""
   )

add_dependencies(djvulibre libjpeg)
include_directories(${djvulibre_prefix})
add_library(djvulibre_lib STATIC IMPORTED)
add_dependencies(djvulibre_lib djvulibre)
set_target_properties(djvulibre_lib PROPERTIES IMPORTED_LOCATION ${djvulibre_lib_dir}/libdjvulibre.so)
#install(DIRECTORY ${djvulibre_lib_dir}
#    DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
#    FILES_MATCHING
#    PATTERN "libdjvulibre.so*"
#    )
#install(DIRECTORY ${turbojpeg_lib_dir}
#    DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
#    FILES_MATCHING
#    PATTERN "libjpeg?so*"
#    )
install(FILES
    ${djvulibre_lib_dir}/libdjvulibre.so
    ${djvulibre_lib_dir}/libdjvulibre.so.21
    ${djvulibre_lib_dir}/libdjvulibre.so.21.7.0
    ${turbojpeg_lib_dir}/libjpeg.so
    ${turbojpeg_lib_dir}/libjpeg.so.62
    ${turbojpeg_lib_dir}/libjpeg.so.62.3.0
    DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR})
# end of libdjvu

#set(CALLIGRA_QML_PLUGIN_DIR ${CMAKE_INSTALL_LIBDIR}/calligra CACHE PATH "The location of the Calligra QtQuick Components")
configure_file(config.h.in config.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fPIC -pie -rdynamic -Wall -Wl,-rpath=/usr/share/${APP_NAME}/lib")

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
    qml/AboutPage.qml
)

file(GLOB plugin_QML_SRCS plugin/*.qml)

set(djvuviewer_TS_SRCS
    ${djvuviewer_SRCS}
    ${djvuviewer_QML_SRCS}
    ${plugin_QML_SRCS}
    qml/translations/StoreDescription.qml
)

create_translation(engen_qm_file ${CMAKE_BINARY_DIR}/djvuviewer.ts ${djvuviewer_TS_SRCS})

add_executable(${APP_NAME} ${djvuviewer_SRCS} ${engen_qm_file})
qt5_use_modules(${APP_NAME} Quick DBus)
target_link_libraries(${APP_NAME} stdc++ ${QT_LIBRARIES} ${BOOSTER_LIBRARY})

install(TARGETS ${APP_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES
    ${APP_NAME}.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES ${djvuviewer_QML_SRCS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${APP_NAME}/qml
)
install(FILES ${engen_qm_file} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${APP_NAME}/translations)
#install(FILES ${CMAKE_BINARY_DIR}/djvu-viewer.ts DESTINATION ${CMAKE_INSTALL_PREFIX}/share/translations/source)

install(FILES icons/86x86/${APP_NAME}.png
DESTINATION share/icons/hicolor/86x86/apps)

install(FILES icons/108x108/${APP_NAME}.png
DESTINATION share/icons/hicolor/108x108/apps)

install(FILES icons/128x128/${APP_NAME}.png
DESTINATION share/icons/hicolor/128x128/apps)

install(FILES icons/172x172/${APP_NAME}.png
DESTINATION share/icons/hicolor/172x172/apps)
