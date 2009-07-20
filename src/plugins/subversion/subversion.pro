TEMPLATE = lib
TARGET = Subversion

include(../../qtcreatorplugin.pri)
include(../../plugins/projectexplorer/projectexplorer.pri)
include(../../plugins/texteditor/texteditor.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/vcsbase/vcsbase.pri)
include(../../libs/utils/utils.pri)

HEADERS += annotationhighlighter.h \
    subversionplugin.h \
    subversioncontrol.h \
    subversionoutputwindow.h \
    settingspage.h \
    subversioneditor.h \
    subversionsubmiteditor.h \
    subversionsettings.h \
    checkoutwizard.h \
    checkoutwizardpage.h

SOURCES += annotationhighlighter.cpp \
    subversionplugin.cpp \
    subversioncontrol.cpp \
    subversionoutputwindow.cpp \
    settingspage.cpp \
    subversioneditor.cpp \
    subversionsubmiteditor.cpp \
    subversionsettings.cpp \
    checkoutwizard.cpp \
    checkoutwizardpage.cpp

FORMS += settingspage.ui \
     checkoutwizardpage.ui

RESOURCES += subversion.qrc

OTHER_FILES += Subversion.pluginspec
