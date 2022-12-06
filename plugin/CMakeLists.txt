find_package(PkgConfig REQUIRED)
pkg_check_modules(SAILFISHSILICA sailfishsilica REQUIRED)
pkg_check_modules(ICU icu-i18n REQUIRED)

include_directories(
    ${SAILFISHSILICA_INCLUDE_DIRS}
    ${ICU_INCLUDE_DIRS}
)

set(plugin_SRCS
    djvuviewerplugin.cpp
)

add_library(djvuviewerplugin MODULE ${plugin_SRCS})
qt5_use_modules(djvuviewerplugin Quick)
target_link_libraries(djvuviewerplugin stdc++ ${QT_LIBRARIES} ${SAILFISHSILICA_LIBRARIES} ${ICU_LIBRARIES})

install(TARGETS djvuviewerplugin DESTINATION /usr/share/${APP_NAME}/qml/DjVu/Viewer)

install(FILES
    qmldir
    ContextMenuHook.qml
    ControllerFlickable.qml
    DeleteButton.qml
    DetailsPage.qml
    DJVUAnnotationEdit.qml
    DJVUAnnotationNew.qml
    DJVUContextMenuHighlight.qml
    DJVUContextMenuLinks.qml
    DJVUContextMenuText.qml
    DJVUDocumentPage.qml
    DJVUDocumentToCPage.qml
    DJVUSelectionDrag.qml
    DJVUSelectionHandle.qml
    DJVUSelectionView.qml
    DJVUStorage.js
    DJVUView.qml
    DocumentFlickable.qml
    DocumentHeader.qml
    DocumentPage.qml
    FullscreenError.qml
    IndexButton.qml
    OverlayToolbar.qml
    SearchBarItem.qml
    ShareButton.qml
    ToolBar.qml
    DESTINATION /usr/share/${APP_NAME}/qml/DjVu/Viewer
)
