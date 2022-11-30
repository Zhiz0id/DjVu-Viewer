/*
 * Copyright (C) 2016 Caliste Damien.
 * Copyright (C) 2022 Yura Beznos <yura.beznos@you-ra.info>
 * Contact: Damien Caliste <dcaliste@free.fr>
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
import DjVu.Viewer.DJVU 1.0 as DJVU

Page {
    id: root

    property variant annotation

    property bool _isText: annotation && (annotation.type == DJVU.Annotation.Text
                                          || annotation.type == DJVU.Annotation.Caret)

    signal remove()

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: content.height

        PullDownMenu {
            MenuItem {
                //% "Delete"
                text: qsTrId("djvu-viewer-mi-delete-annotation")
                onClicked: root.remove()
            }
        }

        Column {
            id: content
            width: parent.width
            PageHeader {
                id: pageHeader
                title: annotation && annotation.author != ""
                       ? annotation.author
                       : (_isText
                          //% "Note"
                          ? qsTrId("djvu-viewer-hd-text-annotation")
                          //% "Comment"
                          : qsTrId("djvu-viewer-hd-comment-annotation"))
            }
            TextArea {
                id: areaContents
                width: parent.width
                height: Math.max(flickable.height - pageHeader.height, implicitHeight)
                background: null
                focus: false
                text: annotation ? annotation.contents : ""
                placeholderText: _isText
                                 //% "Write a note…"
                                 ? qsTrId("djvu-viewer-ta-text-annotation-edit")
                                 //% "Write a comment…"
                                 : qsTrId("djvu-viewer-ta-comment-annotation-edit")
                onTextChanged: {
                    if (annotation) {
                        annotation.contents = text
                    }
                }
            }
        }
        VerticalScrollDecorator { flickable: flickable }
    }
}
