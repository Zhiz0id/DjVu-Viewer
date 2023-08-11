
# rename in files
for f in 'CMakeLists.txt' 'cmake/sailfish_i486.cmake' 'cmake/sailfish_i486.cmake' 'config.h.in' 'qml/FileListPage.qml' 'qml/Main.qml' 'rpm/djvu-viewer.spec' 'info.you-ra.djvuviewer.desktop' 'main.cpp'; do sed -i s#info.you-ra#harbour-info.you-ra# $f; done

# rename files
for f in `find ./ -name "info.you-ra.djvuviewer*"`; do mv $f "$(echo "$f" | sed s#info#harbour-info#)"; done

sed -i '/ExecDBus/d' harbour-info.you-ra.djvuviewer.desktop


