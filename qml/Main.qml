/*
 * Copyright (c) 2013 - 2022 Jolla Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import DjVu.Viewer 1.0
import DjVu.Viewer.Files 1.0

ApplicationWindow {
    id: window

    readonly property Component coverPreview: pageStack.currentPage && (pageStack.currentPage.preview || null)

    property QtObject fileListModel: trackerProvider.model
    property Page _mainPage

    allowedOrientations: defaultAllowedOrientations
    _defaultLabelFormat: Text.PlainText
    _defaultPageOrientations: Orientation.All
    cover: Qt.resolvedUrl("CoverPage.qml")
    initialPage: Component {
        FileListPage {
            id: fileListPage

            model: trackerProvider.model
            provider: trackerProvider
            Component.onCompleted: window._mainPage = fileListPage
        }
    }

    TrackerDocumentProvider {
        id: trackerProvider
    }

    FileInfo {
        id: fileInfo
    }

    // file = file or url
    function openFile(file) {
        fileInfo.url = file

        pageStack.pop(window._mainPage, PageStackAction.Immediate)

        var handler = ""

        switch (fileInfo.mimeType) {
        case "image/vnd.djvu":
        case "image/vnd.djvu+multipage":
        case "image/x.djvu":
        case "image/x-djvu":
            handler = "file:///usr/share/info.you-ra.djvuviewer/qml/DjVu/Viewer/DJVUDocumentPage.qml"
            break

        default:
            console.log("Warning: Unrecognised file type for file " + fileInfo.file)
        }

        if (handler != "") {
            pageStack.push(handler,
                           { title: fileInfo.fileName, source: fileInfo.url, mimeType: fileInfo.mimeType },
                           PageStackAction.Immediate)
        }

        activate()
    }

    function mimeToIcon(fileMimeType) {
        var iconType = "other"
        switch (fileMimeType) {
        case "text/x-vnote":
            iconType = "note"
            break
        case "application/pdf":
            iconType = "pdf"
            break
        case "application/vnd.oasis.opendocument.spreadsheet":
        case "application/x-kspread":
        case "application/vnd.ms-excel":
        case "text/csv":
        case "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet":
        case "application/vnd.openxmlformats-officedocument.spreadsheetml.template":
            iconType = "spreadsheet"
            break
        case "application/vnd.oasis.opendocument.presentation":
        case "application/vnd.oasis.opendocument.presentation-template":
        case "application/x-kpresenter":
        case "application/vnd.ms-powerpoint":
        case "application/vnd.openxmlformats-officedocument.presentationml.presentation":
        case "application/vnd.openxmlformats-officedocument.presentationml.template":
            iconType = "presentation"
            break
        case "text/plain":
        case "application/vnd.oasis.opendocument.text-master":
        case "application/vnd.oasis.opendocument.text":
        case "application/vnd.oasis.opendocument.text-template":
        case "application/msword":
        case "application/rtf":
        case "application/x-mswrite":
        case "application/vnd.openxmlformats-officedocument.wordprocessingml.document":
        case "application/vnd.openxmlformats-officedocument.wordprocessingml.template":
        case "application/vnd.ms-works":
            iconType = "formatted"
            break
        }
        return "image://theme/icon-m-file-" + iconType
    }
}
