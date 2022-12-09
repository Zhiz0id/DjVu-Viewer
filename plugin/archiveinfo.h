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

#ifndef SAILFISH_ARCHIVE_INFO_H
#define SAILFISH_ARCHIVE_INFO_H

#include <QObject>
#include <QString>

namespace Sailfish {

class ArchiveInfoPrivate;

class ArchiveInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString baseName READ baseName NOTIFY baseNameChanged)
    Q_PROPERTY(QString completeSuffix READ completeSuffix NOTIFY completeSuffixChanged)
    Q_PROPERTY(Format format READ format NOTIFY formatChanged)
    Q_PROPERTY(bool supported READ supported NOTIFY supportedChanged)

public:
    explicit ArchiveInfo(QObject *parent = nullptr);
    ArchiveInfo(const QString &file, QObject *parent = nullptr);
    ArchiveInfo(const ArchiveInfo &other);
    ArchiveInfo &operator=(const ArchiveInfo &other);

    ~ArchiveInfo();

    enum Format {
        Unknown,
        Zip,
        Tar,
        SevenZip
    };

    QString file() const;
    void setFile(const QString &file);

    QString fileName() const;
    QString baseName() const;
    QString completeSuffix() const;

    Format format() const;

    bool supported() const;

    Q_INVOKABLE bool exists() const;

    static bool exists(const QString &archiveName);
    static bool supported(const QString &archiveName);

signals:
    void fileChanged();
    void fileNameChanged();
    void baseNameChanged();
    void completeSuffixChanged();
    void formatChanged();
    void supportedChanged();

private:
    ArchiveInfoPrivate *d;
};

}

#endif // SAILFISH_ARCHIVE_INFO_H
