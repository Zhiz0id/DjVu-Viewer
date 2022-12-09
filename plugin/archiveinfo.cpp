/*
 * Copyright (C) 2018 Jolla Ltd.
 * Contact: Raine Mäkeläinen <raine.makelainen@jolla.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Jolla Ltd. nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "archiveinfo.h"

#include <QFileInfo>
#include <QMimeDatabase>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcArchiveInfoLog, "org.sailfishos.archiveinfo", QtWarningMsg)

namespace Sailfish {

class ArchiveInfoPrivate
{
public:
    ArchiveInfoPrivate();

    void updateInfo(const QString &name);

    QString name;
    QFileInfo info;
    ArchiveInfo::Format format;
    bool supported;
};

ArchiveInfoPrivate::ArchiveInfoPrivate()
    : format(ArchiveInfo::Unknown)
    , supported(false)
{
}

void ArchiveInfoPrivate::updateInfo(const QString &name)
{
    QMimeDatabase mimeDatabase;
    QMimeType mt = mimeDatabase.mimeTypeForFile(name);
    qCDebug(lcArchiveInfoLog) << "Archive name:" << name << "mime/type:" << mt.name() << "inherits:" << "parent mime/types:" << mt.parentMimeTypes() ;

    QString mimeType = mt.name();
    QStringList parentTypes = mt.parentMimeTypes();

    supported = true;

    if (mimeType == QLatin1String("application/x-7z-compressed")) {
        format = ArchiveInfo::SevenZip;
    } else if (mimeType == QLatin1String("application/zip")
               || parentTypes.contains(QLatin1String("application/zip"))) {
        format = ArchiveInfo::Zip;
    } else if (mimeType == QLatin1String("application/x-tar") ||
               parentTypes.contains(QLatin1String("application/x-xz")) ||
               parentTypes.contains(QLatin1String("application/gzip")) ||
               parentTypes.contains(QLatin1String("application/x-gzip")) ||
               parentTypes.contains(QLatin1String("application/x-bzip")) ||
               parentTypes.contains(QLatin1String("application/x-bzip2"))) {
        format = ArchiveInfo::Tar;
    } else {
        qCDebug(lcArchiveInfoLog) << "Unsupported archive format mimeType:" << mimeType << "parent types:" << parentTypes;
        format = ArchiveInfo::Unknown;
        supported = false;
    }

    if (supported) {
        info.setFile(name);;
    } else {
        info.setFile(QLatin1String(""));
    }
}

ArchiveInfo::ArchiveInfo(QObject *parent)
    : QObject(parent)
    , d(new ArchiveInfoPrivate)
{
}

ArchiveInfo::ArchiveInfo(const QString &name, QObject *parent)
    : QObject(parent)
    , d(new ArchiveInfoPrivate)
{
    d->updateInfo(name);
}

ArchiveInfo::ArchiveInfo(const ArchiveInfo &other)
    : QObject(nullptr)
    , d(new ArchiveInfoPrivate)
{
    d->updateInfo(other.file());
}

ArchiveInfo &ArchiveInfo::operator=(const ArchiveInfo &other)
{
    if (&other == this)
        return *this;

    d->updateInfo(other.file());
    return *this;
}

ArchiveInfo::~ArchiveInfo()
{
    delete d;
    d = nullptr;
}

QString ArchiveInfo::file() const
{
    return d->info.absoluteFilePath();
}

void ArchiveInfo::setFile(const QString &name)
{
    if (file() != name) {
        bool wasSupported = d->supported;
        ArchiveInfo::Format oldFormat = d->format;

        QString oldFileName = d->info.fileName();
        QString oldBaseName = d->info.baseName();
        QString oldCompleteSuffix = d->info.completeSuffix();

        d->updateInfo(name);
        emit fileChanged();

        if (wasSupported != d->supported) {
            emit supportedChanged();
        }

        if (oldFormat != d->format) {
            emit formatChanged();
        }

        if (oldFileName != d->info.fileName()) {
            emit fileNameChanged();
        }

        if (oldBaseName != d->info.baseName()) {
            emit baseNameChanged();
        }

        if (oldCompleteSuffix != d->info.completeSuffix()) {
            emit completeSuffixChanged();
        }
    }
}

QString ArchiveInfo::fileName() const
{
    return d->info.fileName();
}

QString ArchiveInfo::baseName() const
{
    return d->info.baseName();
}

QString ArchiveInfo::completeSuffix() const
{
    return d->info.completeSuffix();
}

ArchiveInfo::Format ArchiveInfo::format() const
{
    return d->format;
}

bool ArchiveInfo::supported() const
{
    return d->supported;
}

bool ArchiveInfo::exists() const
{
    return d->info.exists();
}

bool ArchiveInfo::exists(const QString &archiveName)
{
    return QFileInfo::exists(archiveName);
}

bool ArchiveInfo::supported(const QString &archiveName)
{
    ArchiveInfo a(archiveName);
    return a.supported();
}

}
