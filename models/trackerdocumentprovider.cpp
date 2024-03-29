/*
 * Copyright (C) 2019 Open Mobile Platform LLC
 * Copyright (C) 2013-2014 Jolla Ltd.
 * Copyright (C) 2023 Yura Beznos
 * Contact: Yura Beznos <yura.beznos@you-ra.info>
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

#include "trackerdocumentprovider.h"
#include "documentlistmodel.h"

#include <QtCore/QModelIndex>
#include <QtCore/QFile>

#include <qglobal.h>

#include "config.h"

/* experiment */
#include <QTemporaryFile>
#include <QDBusInterface>
#include <QDBusUnixFileDescriptor>
#include <QRegularExpression>
#include <QTextStream>
#include <QUrl>
#include "dbusconstants.h"

#include <QDBusArgument>
#include <QDBusReply>

#include <QEventLoop>
#include <QTimer>
inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}
/* end of experiment */

//The query to run to get files out of Tracker.
static const QString documentQuery{
    "PREFIX nfo: <http://tracker.api.gnome.org/ontology/v3/nfo#>"
    "PREFIX nie: <http://tracker.api.gnome.org/ontology/v3/nie#> "
    "SELECT ?name ?path ?size ?lastModified ?mimeType "
    "WHERE {"
    //"  GRAPH tracker:Documents {"
    "{"
    "    SELECT nfo:fileName(?path) AS ?name"
    "      nfo:fileSize(?path) AS ?size"
    "      nfo:fileLastModified(?path) AS ?lastModified"
    "      ?path ?mimeType"
    "    WHERE {"
    "      ?u nie:isStoredAs ?path ."
    "      ?path nie:dataSource/tracker:available true . "
    "      ?u nie:mimeType ?mimeType ."
    "    FILTER (?mimeType IN ('image/vnd.djvu', 'image/vnd.djvu+multipage', 'image/x.djvu', 'image/x-djvu'))"
    "    }"
    "  }"
    "}"
};

static const QString documentGraph("http://tracker.api.gnome.org/ontology/v3/tracker#Documents");

class TrackerDocumentProvider::Private {
public:
    Private()
        : model(new DocumentListModel)
        //, connection{nullptr}
        , ready(false)
        , error(false)
    {
        model->setObjectName("TrackerDocumentList");
    }

    ~Private() {
        model->deleteLater();
    }

    DocumentListModel *model;
    //QSparqlConnection *connection;
    bool ready;
    bool error;
};

TrackerDocumentProvider::TrackerDocumentProvider(QObject *parent)
    : DocumentProvider(parent)
    , d(new Private)
{
}

TrackerDocumentProvider::~TrackerDocumentProvider()
{
    //delete d->connection;
    delete d;
}

void TrackerDocumentProvider::classBegin()
{
}

void TrackerDocumentProvider::componentComplete()
{
    //d->connection = new QSparqlConnection(trackerDriver);
    //if (!d->connection->isValid()) {
    //    qWarning() << "No valid QSparqlConnection on TrackerDocumentProvider";
    //} else {
        startSearch();
    //    d->connection->subscribeToGraph(documentGraph);
    //    connect(d->connection, &QSparqlConnection::graphUpdated,
    //            this, &TrackerDocumentProvider::trackerGraphChanged);
    //}
}

void TrackerDocumentProvider::startSearch()
{

    //QSparqlQuery q(documentQuery);
    //QSparqlResult *result = d->connection->exec(q);
    //if (result->hasError()) {
    //    qWarning() << "Error executing sparql query:" << result->lastError();
    //    delete result;
    //} else {
    //    connect(result, SIGNAL(finished()), this, SLOT(searchFinished()));
    //}
    searchFinished();
}

void TrackerDocumentProvider::stopSearch()
{
}

void TrackerDocumentProvider::searchFinished()
{

    //QSparqlResult *r = qobject_cast<QSparqlResult*>(sender());
    bool wasError = d->error;
    //TODO: add this error
    //d->error = r->hasError();
    d->error = false;

    if (!d->error) {
        // d->model->clear();
        // Mark all current entries in the model dirty.
        d->model->setAllItemsDirty(true);


/* experiment */
    QTemporaryFile tmpFile;
    tmpFile.open();
    QDBusUnixFileDescriptor buffer(tmpFile.handle());
    QMap<QString, QVariant> answerMap;
    static const QString method(QStringLiteral("Query"));
    QDBusInterface dbus_iface(TRACKER_SERVICE, TRACKER_PATH, TRACKER_INTERFACE, QDBusConnection::sessionBus());
    QDBusReply<QStringList> replay = dbus_iface.call(method, documentQuery /*SPARQL_QUERY*/, QVariant::fromValue(buffer), answerMap);

    const auto returnValue = replay.value();

    QStringList freshData;
    QTextStream stream(&tmpFile);
    QString result;
    QString tmp;
    while (true) {
        delay(10);
        stream.seek(0);
        auto string = stream.readAll();
        if (string.length() > 0) {
            tmp = string;
            result.append(string);
        } else {
            break;
        }
        if (string.length() > 0 && string.at(string.length()-1) == QChar('\0')) {
            break;
        }
    }
    tmpFile.close();

    while (result.length() > 0) {
        auto numOfFields = 5;
        result.remove(0,44);

        QStringList fileInfo;

        while (numOfFields > 0) {
            numOfFields--;
            auto idx = result.indexOf(QChar('\0'));
            auto first = result.left(idx);
            fileInfo << first;
            result.remove(0,idx+1);
        }
        d->model->addItem(
            fileInfo[0],
            fileInfo[1],
            fileInfo[1].split('.').last(),
            fileInfo[2].toInt(),
            QDateTime::fromString(fileInfo[3], Qt::ISODate),
            fileInfo[4]
        );

    }


/* end of experiment */

        //while (r->next()) {
            // This will remove the dirty flag for already
            // existing entries.
/*            d->model->addItem(
                r->binding(0).value().toString(),
                r->binding(1).value().toString(),
                r->binding(1).value().toString().split('.').last(),
                r->binding(2).value().toInt(),
                r->binding(3).value().toDateTime(),
                r->binding(4).value().toString()
            );*/
        //}
        // Remove all entries with the dirty mark.
        d->model->removeItemsDirty();
        if (!d->ready) {
            d->ready = true;
            emit readyChanged();
        }
    }

    //delete r;

    if (wasError != d->error) {
        emit errorChanged();
    }

    emit countChanged();
}

int TrackerDocumentProvider::count() const
{
    // TODO lolnope
    return d->model->rowCount(QModelIndex());
}

bool TrackerDocumentProvider::isReady() const
{
    return d->ready;
}

bool TrackerDocumentProvider::error() const
{
    return d->error;
}

QObject* TrackerDocumentProvider::model() const
{
    return d->model;
}

void TrackerDocumentProvider::deleteFile(const QUrl &file)
{
    if (QFile::exists(file.toLocalFile())) {
        QFile::remove(file.toLocalFile());

        const int count = d->model->rowCount(QModelIndex());
        for (int i = 0; i < count; ++i) {
            if (d->model->data(d->model->index(i, 0), DocumentListModel::FilePathRole).toUrl() == file) {
                d->model->removeAt(i);
                break;
            }
        }
    }
}

void TrackerDocumentProvider::trackerGraphChanged(const QString &graphName)
{
    if (graphName == documentGraph) {
        startSearch();
    }
}
