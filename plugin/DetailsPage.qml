/*
 * Copyright (C) 2013-2014 Jolla Ltd.
 * Contact: Robin Burchell <robin.burchell@jolla.com>
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
import Nemo.FileManager 1.0

Page {
    id: page

    property url source
    property int indexCount
    property string mimeType

    FileInfo {
        id: info
        url: page.source
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: contentColumn.height + Theme.paddingLarge

        Column {
            id: contentColumn
            width: parent.width

            PageHeader {
                id: detailsHeader
                //: Details page title
                //% "Details"
                title: qsTrId("djvu-viewer-he-details")
            }

            DetailItem {
                //: File path detail of the document
                //% "File path"
                label: qsTrId("djvu-viewer-la-filepath")
                value: info.file
                alignment: Qt.AlignLeft
            }

            DetailItem {
                //: File size detail of the document
                //% "Size"
                label: qsTrId("djvu-viewer-la-filesize")
                value: Format.formatFileSize(info.size)
                alignment: Qt.AlignLeft
            }

            DetailItem {
                //: File type detail of the document
                //% "Type"
                label: qsTrId("djvu-viewer-la-filetype")
                value: info.mimeTypeComment
                alignment: Qt.AlignLeft
            }

            DetailItem {
                //: Last modified date of the document
                //% "Last modified"
                label: qsTrId("djvu-viewer-la-lastmodified")
                value: Format.formatDate(info.lastModified, Format.DateFull)
                alignment: Qt.AlignLeft
            }

            DetailItem {
                label: {
                    switch (page.mimeType) {
                        case "image/vnd.djvu":
                        case "image/vnd.djvu+multipage":
                        case "image/x.djvu":
                        case "image/x-djvu":
                            //: Page count of the text document
                            //% "Page Count"
                            return qsTrId("djvu-viewer-la-pagecount")
                        default:
                            //: Index count for unknown document types.
                            //% "Index Count"
                            return qsTrId("djvu-viewer-la-indexcount")
                    }
                }
                value: page.indexCount
                alignment: Qt.AlignLeft
            }
        }

        VerticalScrollDecorator {}
    }
}
