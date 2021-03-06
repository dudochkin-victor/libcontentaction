/*
 * Copyright (C) 2009, 2010 Nokia Corporation.
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
#include "service.h"

#include <MDesktopEntry>
#include <QFileInfo>

/*!
  \class ContentAction::Action

  \brief ContentAction::Action represents an action for the given resources

  The Action object binds together the method of invocation (e.g. which D-Bus
  method to call, what binary to execute) and the resources (URIs, Tracker
  objects, text snippets, etc.) used for its creation.

  There are multiple ways to construct an Action:

  - For <b>objects in Tracker</b> try Action::actions() and
    Action::defaultAction()
  - For handling ordinary \b files and other URIs call
    Action::actionsForFile() or Action::defaultActionForFile()
  - For <b>pseudo-urls</b> like \c "mailto:" use Action::actionsForScheme() or
    Action::defaultActionForScheme()

  Functions returning multiple actions try to return them in the order of
  relevance.  The action can be triggered by calling the trigger() method.

  Some functions accept multiple inputs (a list), they will return the
  intersection of applicable actions for all inputs.  If the actions for the
  inputs would conflict, these methods return an empty result set (emtpy list
  or an invalid Action).

*/
namespace ContentAction {

using namespace ContentAction::Internal;

namespace Internal {
const QString XMaemoServiceKey("Desktop Entry/X-Maemo-Service");
const QString XOssoServiceKey("Desktop Entry/X-Osso-Service");
const QString XMaemoMethodKey("Desktop Entry/X-Maemo-Method");
const QString XMaemoObjectPathKey("Desktop Entry/X-Maemo-Object-Path");
const QString ExecKey("Desktop Entry/Exec");
}

ActionPrivate::~ActionPrivate()
{
}

bool ActionPrivate::isValid() const
{
    return false;
}

QString ActionPrivate::name() const
{
    return QString("Invalid action");
}

QString ActionPrivate::localizedName() const
{
    return name();
}

QString ActionPrivate::icon() const
{
    LCA_WARNING << "called icon() for something that doesn't implement it";
    return "NOT_IMPLEMENTED";
}

void ActionPrivate::trigger(bool) const
{
    LCA_WARNING << "triggered an invalid action, not doing anything.";
}

DefaultPrivate::DefaultPrivate(QSharedPointer<MDesktopEntry> desktopEntry,
                               const QStringList& params, bool valid)
    : desktopEntry(desktopEntry), params(params), valid(valid)
{
}

DefaultPrivate::~DefaultPrivate()
{
}

bool DefaultPrivate::isValid() const
{
    return valid;
}

QString DefaultPrivate::name() const
{
    return QFileInfo(desktopEntry->fileName()).baseName();
}

QString DefaultPrivate::localizedName() const
{
    return desktopEntry->name();
}

QString DefaultPrivate::icon() const
{
    return desktopEntry->icon();
}

Action::Action()
    : d(new ActionPrivate())
{
}

Action::Action(const Action& other)
    : d(other.d)
{
}

Action& Action::operator=(const Action& other)
{
    d = other.d;
    return *this;
}

Action::Action(ActionPrivate* priv)
    : d(priv)
{
}

/// Creates an Action object which will launch the application defined by \a
/// desktopFilePath with the given \a params when triggered.
Action createAction(const QString& desktopFilePath, const QStringList& params)
{
    QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(
                                                   desktopFilePath));
    return createAction(desktopEntry, params);
}

/// Creates an Action object which will launch the application defined by \a
/// desktopEntry with the given \a params when triggered.
Action createAction(QSharedPointer<MDesktopEntry> desktopEntry,
                    const QStringList& params)
{
    if (desktopEntry->contains(XMaemoMethodKey) &&
        !desktopEntry->contains(XMaemoServiceKey)) {
        return Action(new ServiceFwPrivate(desktopEntry, params));
    }
    else if (desktopEntry->contains(XMaemoServiceKey) ||
             desktopEntry->contains(XOssoServiceKey)) {
        return Action(new DBusPrivate(desktopEntry, params));
    }
    else if (desktopEntry->contains(ExecKey)) {
        return Action(new ExecPrivate(desktopEntry, params));
    }
    else {
        // We don't know how to launch
        return Action(new DefaultPrivate(desktopEntry, params, false));
    }
}

Action::~Action()
{
}

/// Triggers the action represented by this object, using the URIs contained
/// by the Action object.
void Action::trigger() const
{
    d->trigger(false);
}

/// Triggers the action represented by this object, using the URIs contained by
/// the Action object.  Waits for the application handling the message to
/// respond, if possible.  Otherwise, does the same as trigger().  Waiting is
/// only possible when the application is launched via D-Bus.
void Action::triggerAndWait() const
{
    d->trigger(true);
}

/// Returns \a true if the Action object represents an action which can be
/// triggered.
bool Action::isValid() const
{
    return d->isValid();
}

/// Returns the name of the action (the basename of the .desktop file
/// describing the action).
QString Action::name() const
{
    return d->name();
}

/// Returns the localized name of the action.  Note that if the locale
/// changes, actions must be recreated.
QString Action::localizedName() const
{
    return d->localizedName();
}

/// Returns the icon name for the action.
QString Action::icon() const
{
    return d->icon();
}

/// Creates an action that will launch the given application (specified by
/// .desktop file name) with the \a params the way the application specifies in
/// their .desktop file. \a app is the name of the .desktop file (with the
/// .desktop extension). It is looked for in the standard locations.
Action Action::launcherAction(const QString& app, const QStringList& params)
{
    QString appDesktop = findDesktopFile(app);
    if (!appDesktop.isEmpty()) {
        return createAction(appDesktop, params);
    }
    return Action();
}

/// Creates an action that will launch the given application (specified by
/// MDesktopEntry) with the \a params the way the application specifies in
/// their .desktop file.
Action Action::launcherAction(QSharedPointer<MDesktopEntry> mDesktop, const QStringList& params)
{
    return createAction(mDesktop, params);
}

} // end namespace
