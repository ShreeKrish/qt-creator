/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "buildaspects.h"

#include <utils/fileutils.h>
#include <utils/utilsicons.h>

#include <QLabel>
#include <QLayout>

using namespace Utils;

namespace ProjectExplorer {

class BuildDirectoryAspect::Private
{
public:
    FilePath sourceDir;
    FilePath savedShadowBuildDir;
    QString problem;
    QPointer<QLabel> warningLabel;
    QPointer<QLabel> problemLabel;
};

BuildDirectoryAspect::BuildDirectoryAspect() : d(new Private)
{
    setSettingsKey("ProjectExplorer.BuildConfiguration.BuildDirectory");
    setLabelText(tr("Build directory:"));
    setDisplayStyle(PathChooserDisplay);
    setExpectedKind(Utils::PathChooser::Directory);
}

BuildDirectoryAspect::~BuildDirectoryAspect()
{
    delete d;
}

void BuildDirectoryAspect::allowInSourceBuilds(const FilePath &sourceDir)
{
    d->sourceDir = sourceDir;
    makeCheckable(CheckBoxPlacement::Top, tr("Shadow build:"), QString());
}

bool BuildDirectoryAspect::isShadowBuild() const
{
    return !d->sourceDir.isEmpty() && d->sourceDir != filePath();
}

void BuildDirectoryAspect::setProblem(const QString &description)
{
    d->problem = description;
    updateProblemLabel();
}

void BuildDirectoryAspect::toMap(QVariantMap &map) const
{
    BaseStringAspect::toMap(map);
    if (!d->sourceDir.isEmpty()) {
        const FilePath shadowDir = isChecked() ? filePath() : d->savedShadowBuildDir;
        map.insert(settingsKey() + ".shadowDir", shadowDir.toString());
    }
}

void BuildDirectoryAspect::fromMap(const QVariantMap &map)
{
    BaseStringAspect::fromMap(map);
    if (!d->sourceDir.isEmpty()) {
        d->savedShadowBuildDir = FilePath::fromString(map.value(settingsKey() + ".shadowDir")
                                                      .toString());
        setChecked(d->sourceDir != filePath());
    }
}

void BuildDirectoryAspect::addToLayout(LayoutBuilder &builder)
{
    BaseStringAspect::addToLayout(builder);
    d->warningLabel = new QLabel;
    d->warningLabel->setAlignment(Qt::AlignTop);
    d->warningLabel->setPixmap(Icons::WARNING.pixmap());
    d->problemLabel = new QLabel;
    d->problemLabel->setAlignment(Qt::AlignTop);
    builder.startNewRow().addItems(QString(), d->warningLabel.data(), d->problemLabel.data());
    updateProblemLabel();
    if (!d->sourceDir.isEmpty()) {
        connect(this, &BaseStringAspect::checkedChanged, builder.layout(), [this] {
            if (isChecked()) {
                setFilePath(d->savedShadowBuildDir.isEmpty()
                            ? d->sourceDir : d->savedShadowBuildDir);
            } else {
                d->savedShadowBuildDir = filePath();
                setFilePath(d->sourceDir);
            }
        });
    }
}

void BuildDirectoryAspect::updateProblemLabel()
{
    if (!d->warningLabel)
        return;
    QTC_ASSERT(d->problemLabel, return);
    d->problemLabel->setText(d->problem);
    d->problemLabel->setVisible(!d->problem.isEmpty());
    d->warningLabel->setVisible(!d->problem.isEmpty());
}

SeparateDebugInfoAspect::SeparateDebugInfoAspect()
{
    setDisplayName(tr("Separate Debug Info"));
    setSettingsKey("SeparateDebugInfo");
}

} // namespace ProjectExplorer