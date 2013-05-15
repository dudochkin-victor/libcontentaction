/*
 * Copyright (C) 2010 Nokia Corporation.
 *
 * Contact: Marius Vollmer <marius.vollmer@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */
#include "internal.h"

#include <QDebug>
#include <QRegExp>
#include <QDBusInterface>
#include <QCoreApplication>

namespace ContentAction {

using namespace ContentAction::Internal;

/// Highlights fragments of \a text which have applicable actions.  Returns a
/// list of Match objects.
QList<Match> Action::highlight(const QString& text)
{
    const QList<QPair<QString, QString> >& cfg = highlighterConfig();
    QList<Match> result;

    for (int i = 0; i < cfg.size(); ++i) {
        QRegExp re(cfg[i].second, Qt::CaseInsensitive);
        QStringList apps = appsForContentType(cfg[i].first);
        int pos = 0;
        while ((pos = re.indexIn(text, pos)) != -1) {
            int l = re.matchedLength();
            Match m;
            m.start = pos;
            m.end = pos + l;

            Q_FOREACH (const QString& app, apps) {
                m.actions << createAction(findDesktopFile(app), QStringList() << re.cap());
            }
            result << m;
            pos += l;
            if (l == 0)
                ++pos;
        }
    }
    return result;
}

bool Match::operator<(const Match& other) const
{
    return (this->start < other.start) ||
        ((this->start == other.start) && (this->end < other.end));
}


} // end namespace
