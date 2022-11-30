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

#include "djvuviewerplugin.h"

#include <QtQml/QtQml>

#include <QTranslator>
#include <QApplication>

//#include "plaintextmodel.h"

#include "config.h"

class Translator : public QTranslator
{
public:
    Translator(QObject *parent)
        : QTranslator(parent)
    {
        qApp->installTranslator(this);
    }

    ~Translator()
    {
        qApp->removeTranslator(this);
    }
};

DjVuViewerPlugin::DjVuViewerPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

void DjVuViewerPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("DjVu.Viewer"));
    //qmlRegisterType<PlainTextModel>(uri, 1, 0, "PlainTextModel");
}

void DjVuViewerPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QLatin1String("DjVu.Viewer"));

    Translator *engineeringEnglish = new Translator(engine);
    engineeringEnglish->load("djvu-viewer_eng_en", TRANSLATION_INSTALL_DIR);

    Translator *translator = new Translator(engine);
    translator->load(QLocale(), "djvu-viewer", "-", TRANSLATION_INSTALL_DIR);

    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

