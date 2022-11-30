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

#include <QApplication>
#include <QGuiApplication>
#include <qqmldebug.h>
#include <QtQml>
#include <QQuickView>
#include <QQmlError>
#include <QQmlEngine>
#include <QQuickItem>
#include <QDBusConnection>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QLoggingCategory>

#include <MDeclarativeCache>

#include "config.h"
#include "models/filtermodel.h"
#include "models/documentlistmodel.h"
#include "models/trackerdocumentprovider.h"
#include "models/documentprovider.h"
#include "dbusadaptor.h"

namespace {
QSharedPointer<QApplication> createApplication(int &argc, char **argv)
{
    auto app = QSharedPointer<QApplication>(new QApplication(argc, argv));
    //FIXME: We should be able to use a pure QGuiApplication but currently too much of
    //Calligra depends on QApplication.
    //QSharedPointer<QGuiApplication>(MDeclarativeCache::qApplication(argc, argv));

    QTranslator *engineeringEnglish = new QTranslator(app.data());
    if (!engineeringEnglish->load("djvu-viewer_eng_en", TRANSLATION_INSTALL_DIR))
        qWarning("Could not load engineering english translation file!");
    QCoreApplication::installTranslator(engineeringEnglish);

    QTranslator *translator = new QTranslator(app.data());
    if (!translator->load(QLocale::system(), "djvu-viewer", "-", TRANSLATION_INSTALL_DIR))
        qWarning() << "Could not load translations for" << QLocale::system().name().toLatin1();
    QCoreApplication::installTranslator(translator);

    return app;
}

QSharedPointer<QQuickView> createView(const QString &file)
{
    qmlRegisterType<DocumentListModel>("DjVu.Viewer.Files", 1, 0, "DocumentListModel");
    qmlRegisterType<TrackerDocumentProvider>("DjVu.Viewer.Files", 1, 0, "TrackerDocumentProvider");
    qmlRegisterType<FilterModel>("DjVu.Viewer.Files", 1, 0, "FilterModel");
    qmlRegisterInterface<DocumentProvider>("DocumentProvider");

    QSharedPointer<QQuickView> view(MDeclarativeCache::qQuickView());
    view->setSource(QUrl::fromLocalFile(QML_INSTALL_DIR + file));

    new DBusAdaptor(view.data());

    if (!QDBusConnection::sessionBus().registerObject("/org/djvu/Viewere", view.data()))
        qWarning() << "Could not register /org/djvu/Viewer D-Bus object.";

    if (!QDBusConnection::sessionBus().registerService("org.djvu.Viewer"))
        qWarning() << "Could not register org.djvu.Viewer D-Bus service.";

    return view;
}
}


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    // TODO: start using Silica booster
    QQuickWindow::setDefaultAlphaBuffer(true);

    if (!qgetenv("QML_DEBUGGING_ENABLED").isEmpty()) {
        QQmlDebuggingEnabler qmlDebuggingEnabler;
    }

    auto app = createApplication(argc, argv);
    // Note, these must be said now, otherwise some plugins using QSettings
    // will get terribly confused when they fail to load properly.
    app->setOrganizationName("org.djvu");
    app->setApplicationName("Viewer");
    auto view = createView("Main.qml");

    //% "Documents"
    Q_UNUSED(QT_TRID_NOOP("djvu-viewer-ap-name"))

    bool preStart = false;
    bool debug = false;
    QString fileName;

    for (int i = 1; i < argc; ++i) {
        QString parameter(argv[i]);
        if (parameter == QStringLiteral("-prestart")) {
            preStart = true;
        } else if (parameter == QStringLiteral("-d")) {
            debug = true;
        } else if (fileName.isEmpty()) {
            fileName = parameter;
        }
    }

    if (!debug) {
        // calligra is quite noisy by default
        QLoggingCategory::setFilterRules("calligra.*.debug=false");
    }

    int retn = 1;
    if (view->errors().count() == 0) {
        if (!fileName.isEmpty()) {
            QVariant fileNameParameter(fileName);
            QMetaObject::invokeMethod(view->rootObject(), "openFile", Q_ARG(QVariant, fileNameParameter));
        } else if (!preStart) {
            view->showFullScreen();
        }

        retn = app->exec();
    }

    return retn;
}
