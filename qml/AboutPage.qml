/*******************************************************************************
**
** Copyright (C) 2023 Yura Beznos at You-ra.info
**
** This file is part of the Screen recorder application project.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write an email to the license@you-ra.info
*******************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    objectName: "aboutPage"
    allowedOrientations: Orientation.All

    SilicaFlickable {
        objectName: "flickable"
        anchors.fill: parent
        contentHeight: layout.height + Theme.paddingLarge

        Column {
            id: layout
            objectName: "layout"
            width: parent.width

            PageHeader {
                objectName: "pageHeader"
                //% "About Application"
                title: qsTrId("djvuviewer-ap-about")
            }

            Label {
                objectName: "descriptionText"
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
                color: palette.highlightColor
                font.pixelSize: Theme.fontSizeSmall
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                //% "<p>DjVu Viewer application</p>"
                text: qsTrId("djvuviewer-ap-description")
            }

            SectionHeader {
                objectName: "licenseHeader"
                //% "GNU General Public License, version 2"
                text: qsTrId("djvuviewer-ap-license-header")
            }

            Label {
                objectName: "licenseText"
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
                color: palette.highlightColor
                font.pixelSize: Theme.fontSizeSmall
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                //% "<p><i>Copyright (C) 2023 Yura Beznos at You-ra.info</i></p><p><i> This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2  of the License, or (at your option) any later version.</i></p> <p><i> This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.</i></p> <p><i> You should have received a copy of the GNU General Public License along with this program; if not, write an email to the license@you-ra.info</i></p>"
                text: qsTrId("djvuviewer-ap-license-text")
            }
        }
    }
}
