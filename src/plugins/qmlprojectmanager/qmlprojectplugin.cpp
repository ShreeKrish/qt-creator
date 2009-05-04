/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
**************************************************************************/

#include "qmlprojectplugin.h"
#include "qmlprojectmanager.h"
#include "qmlprojectwizard.h"
#include "qmlprojectconstants.h"
#include "qmlprojectfileseditor.h"

#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>

#include <texteditor/texteditoractionhandler.h>

#include <QtCore/QtPlugin>
#include <QtCore/QDebug>

using namespace QmlProjectManager;
using namespace QmlProjectManager::Internal;

QmlProjectPlugin::QmlProjectPlugin()
    : m_projectFilesEditorFactory(0)
{ }

QmlProjectPlugin::~QmlProjectPlugin()
{
    removeObject(m_projectFilesEditorFactory);
    delete m_projectFilesEditorFactory;
}

bool QmlProjectPlugin::initialize(const QStringList &, QString *errorMessage)
{
    using namespace Core;

    ICore *core = ICore::instance();
    Core::MimeDatabase *mimeDB = core->mimeDatabase();

    const QLatin1String mimetypesXml(":qmlproject/QmlProject.mimetypes.xml");

    if (! mimeDB->addMimeTypes(mimetypesXml, errorMessage))
        return false;

    Manager *manager = new Manager;

    TextEditor::TextEditorActionHandler *actionHandler =
            new TextEditor::TextEditorActionHandler(Constants::C_FILESEDITOR);

    m_projectFilesEditorFactory = new ProjectFilesFactory(manager, actionHandler);
    addObject(m_projectFilesEditorFactory);

    addAutoReleasedObject(manager);
    addAutoReleasedObject(new QmlProjectWizard);

    return true;
}

void QmlProjectPlugin::extensionsInitialized()
{ }

Q_EXPORT_PLUGIN(QmlProjectPlugin)
