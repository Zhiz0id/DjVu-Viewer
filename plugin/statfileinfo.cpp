/*
 * Copyright (C) 2016 Jolla Ltd.
 * Contact: Joona Petrell <joona.petrell@jollamobile.com>
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

#include "statfileinfo.h"

#include <QMimeDatabase>

StatFileInfo::StatFileInfo() :
    m_selected(false)
{
    refresh();
}

StatFileInfo::StatFileInfo(QString fileName) :
    m_fileName(fileName), m_selected(false)
{
    refresh();
}

StatFileInfo::~StatFileInfo()
{
}

void StatFileInfo::setFile(QString fileName)
{
    if (m_fileName != fileName) {
        m_fileName = fileName;
        refresh();
    }
}

bool StatFileInfo::exists() const
{
    return m_fileInfo.exists();
}

bool StatFileInfo::isSafeToRead() const
{
    // it is safe to read non-existing files
    if (!exists())
        return true;

    // check the file is a regular file and not a special file
    return isFileAtEnd();
}

bool StatFileInfo::isSymLinkBroken() const
{
    // if it is a symlink but it doesn't exist, then it is broken
    if (m_fileInfo.isSymLink() && !m_fileInfo.exists())
        return true;
    return false;
}

void StatFileInfo::setSelected(bool selected)
{
    m_selected = selected;
}

void StatFileInfo::refresh()
{
    memset(&m_stat, 0, sizeof(m_stat));
    memset(&m_lstat, 0, sizeof(m_lstat));

    m_fileInfo = QFileInfo(m_fileName);
    if (m_fileName.isEmpty()) {
        m_mimeType = QMimeType();
        m_baseName = QString();
        m_extension = QString();

        fileChanged();

        return;
    }

    // QMimeDatabase is just a pointer to a global static instance of the actual database so there's
    // no real cost to constructing one when needed.
    QMimeDatabase mimeDatabase;

    m_mimeType = mimeDatabase.mimeTypeForFile(m_fileInfo);
    m_extension = mimeDatabase.suffixForFileName(m_fileName);
    m_baseName = m_fileInfo.fileName();

    if (!m_extension.isEmpty()) {
        if (m_baseName.lastIndexOf(m_extension) < 1) {
            m_extension.clear();
        } else {
            m_baseName.chop(m_extension.length() + 1);
        }
    }

    m_archiveInfo.setFile(m_fileName);

    QByteArray ba = m_fileName.toUtf8();
    char *fn = ba.data();

    // check the file without following symlinks
    int res = lstat64(fn, &m_lstat);
    if (res != 0) { // if error, then set to undefined
        m_lstat.st_mode = 0;
    }
    // if not symlink, then just copy lstat data to stat
    if (!S_ISLNK(m_lstat.st_mode)) {
        memcpy(&m_stat, &m_lstat, sizeof(m_stat));        
    } else {
        // check the file after following possible symlinks
        res = stat64(fn, &m_stat);
        if (res != 0) { // if error, then set to undefined
            m_stat.st_mode = 0;
        }
    }

    fileChanged();
}

bool operator==(const StatFileInfo &lhs, const StatFileInfo &rhs)
{
    // We could just do an inode comparison here?
    return (lhs.fileName() == rhs.fileName() &&
            lhs.size() == rhs.size() &&
            lhs.permissions() == rhs.permissions() &&
            lhs.lastModified() == rhs.lastModified() &&
            lhs.isSymLink() == rhs.isSymLink() &&
            lhs.isDirAtEnd() == rhs.isDirAtEnd());
}

bool operator!=(const StatFileInfo &lhs, const StatFileInfo &rhs)
{
    return !operator==(lhs, rhs);
}

FileInfo::FileInfo(QObject *parent)
    : QObject(parent)
{
}

FileInfo::~FileInfo()
{
}

QUrl FileInfo::url() const
{
    return m_url;
}

void FileInfo::setUrl(const QUrl &url)
{
    if (m_url != url) {
        const bool wasLocal = m_localFile;

        m_url = url;

        if (!m_url.isEmpty() && m_url.isLocalFile()) {
            m_localFile = true;

            StatFileInfo::setFile(m_url.toLocalFile());
        } else {
            m_localFile = false;

            StatFileInfo::setFile(QString());
        }

        if (m_localFile != wasLocal) {
            emit localFileChanged();
        }

        emit urlChanged();
    }
}

bool FileInfo::isLocalFile() const
{
    return m_localFile;
}

void FileInfo::setFile(const QString &file)
{
    if (file.isEmpty()) {
        setUrl(QUrl());
    } else {
        setUrl(QUrl::fromLocalFile(file));
    }
}
