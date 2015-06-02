# Copyright (C) 2015  Christian Höne <christianhoene@cuteseco.org>
#
# This file is part of CuteSeCo.
#
# CuteSeCo is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CuteSeCo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with CuteSeCo.  If not, see <http://www.gnu.org/licenses/>.
#
# Diese Datei ist Teil von CuteSeCo.
#
# CuteSeCo ist Freie Software: Sie können es unter den Bedingungen
# der GNU Affero General Public License, wie von der Free Software Foundation,
# Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
# veröffentlichten Version, weiterverbreiten und/oder modifizieren.
#
# CuteSeCo wird in der Hoffnung, dass es nützlich sein wird, aber
# OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
# Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
# Siehe die GNU Affero General Public License für weitere Details.
#
# Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
# diesem Programm erhalten haben.
# Wenn nicht, siehe <http://www.gnu.org/licenses/>.

PROGNAME="CuteSeCo"
VERSION="0.0.1"
#STATE="final"
#STATE="beta"
#STATE="alpha"
STATE="experimental"


# Qt libraries
QT += core
QT += gui
QT += widgets
QT += network
#QT += sql
#QT += webkitwidgets
#QT += svg
#QT += printsupport
macx: {
    QT += macextras
}


# Directories
DESTDIR     = build
OBJECTS_DIR = tmp/obj
MOC_DIR     = tmp/moc
UI_DIR      = tmp/ui
RCC_DIR     = tmp/rcc


# target config
TARGET = $${PROGNAME}
TEMPLATE = app
CONFIG += static


# Development (debug build)
CONFIG(debug, debug|release) {
    TARGET = "$${TARGET}Developer"
    DEFINES += PROJECT_DEVELOPERMODE
    # windows only:
    DLLSUFFIX = "d"
} else
{
    # windows only:
    DLLSUFFIX = ""
}


# Architecture
linux-g++:QMAKE_TARGET.arch     = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch  = x86
linux-g++-64:QMAKE_TARGET.arch  = x86_64
macx:QMAKE_TARGET.arch          = x86_64


# definitions for scripts
CONCAT = ;

win32: {
    CONCAT = &
}
macx: {
    QMAKE_TARGET_BUNDLE_PREFIX = "com.cuteseco"
}


# some definitions
BUILDNO = $$cat(buildno)

macx: {
    INFO_PLIST_PATH = \
        $$shell_quote($${OUT_PWD}/$${TARGET}.app/Contents/Info.plist)
    QMAKE_POST_LINK += \
        /usr/libexec/PlistBuddy \
        -c \"Set :CFBundleShortVersionString $${VERSION}\" $${INFO_PLIST_PATH}
}


# get git version/tag
GIT_BASE_CMD = "git --git-dir $$PWD/.git --work-tree $$PWD "
GIT_CMD = "$${GIT_BASE_CMD} describe --always --tags"
!win32:GIT_VERSION = $$system($${GIT_CMD} 2> /dev/null)
win32:GIT_VERSION = $$system($${GIT_CMD} 2> NUL)


# export definitions
DEFINES += PROJECT_PROGNAME="'\"$${TARGET}\"'"
DEFINES += PROJECT_VERSION="'\"$${VERSION}\"'"
DEFINES += PROJECT_BUILDNO="'\"$${BUILDNO}\"'"
DEFINES += PROJECT_GIT_VERSION="'\"$${GIT_VERSION}\"'"
DEFINES += PROJECT_STATE="'\"$${STATE}\"'"
message("$${TARGET}    "\
        "Version: $${VERSION}    "\
        "Build: $${BUILDNO}    "\
        "Git: $${GIT_VERSION}    "\
        "Arch: $${QMAKE_TARGET.arch}    "\
        "(Qt $${QT_VERSION}) "\
        )


# increment build no
increment-buildno.commands = @echo INFO: incrementing build no $${CONCAT}
unix:increment-buildno.commands += \
    $${PWD}/SCRIPTS/increment-buildno $${PWD}/buildno $${CONCAT}
win32:increment-buildno.commands += \
    $${PWD}/SCRIPTS/increment-buildno.bat $${PWD}/buildno $${CONCAT}
PRE_TARGETDEPS += increment-buildno
QMAKE_EXTRA_TARGETS += increment-buildno


# copy scripts to build dir
copyscripts.commands = @echo INFO: copying scripts $${CONCAT}
!win32:copyscripts.commands += \
    $(COPY_DIR) \"$${PWD}/SCRIPTS\" \"$${DESTDIR}/\"
win32:copyscripts.commands  += \
    $(COPY_DIR) \"$${PWD}/SCRIPTS\" \"$${DESTDIR}/SCRIPTS\"
export(copyscripts.commands)
first.depends += copyscripts
QMAKE_EXTRA_TARGETS += copyscripts


# copy translations to build dir
copytranslations.commands = @echo INFO: copying translations $${CONCAT}
!macx:!win32:copytranslations.commands += \
    $(COPY_DIR) \"$${PWD}/translations\" \"$${DESTDIR}/\"
win32:copytranslations.commands += \
    $(COPY_DIR) \"$${PWD}/translations\" \"$${DESTDIR}/translations\"
macx:copytranslations.commands  += \
    $(COPY_DIR) \
    \"$${PWD}/translations\" \
    \"$${DESTDIR}/$${TARGET}.app/Contents/MacOS/\"
export(copytranslations.commands)
first.depends += copytranslations
QMAKE_EXTRA_TARGETS += copytranslations


# strip unneeded
stripunneeded.commands = @echo INFO: removing ballast $${CONCAT}
unix:!macx {
    stripunneeded.commands += \
        strip --strip-unneeded $${DESTDIR}/$${TARGET} $${CONCAT}
    stripunneeded.commands += \
        strip --remove-section=.comment $${DESTDIR}/$${TARGET} $${CONCAT}
}
macx: {
    stripunneeded.commands += \
        dsymutil \
        $${TARGET}.app/Contents/MacOS/$${TARGET} \
        -o $${TARGET}.app.dSYM $${CONCAT}
}
win32: {
    # do nothing, be fat
}
CONFIG(release, debug|release) {
    first.depends += stripunneeded
    export(first.depends)
}
QMAKE_EXTRA_TARGETS += stripunneeded


# copy binary
win32: {
    copybinary.commands = @echo INFO: copying binary $${CONCAT}
    copybinary.commands += \
        $(COPY) \"$${DESTDIR}\\$${TARGET}.exe\" \"Z:\\$${TARGET}.exe\"
    first.depends += copybinary
    export(copybinary.commands)
    QMAKE_EXTRA_TARGETS += copybinary
}


# compiler settings
unix: {
    target.path = /usr/bin

    INSTALLS += target
}

unix:!macx {
    #QMAKE_LFLAGS_RPATH=
    #QMAKE_LFLAGS = -Wl,--rpath=\\\$\$ORIGIN/,--rpath=\\\$\$ORIGIN/lib
}

macx: {
    ICON = images/icons/$${TARGET}.icns

    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_OBJECTIVE_CFLAGS_RELEASE = \
        $$QMAKE_OBJECTIVE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32: {
    RC_FILE = $${TARGET}.rc
}

message(extra targets: $$QMAKE_EXTRA_TARGETS)
export(first.depends)
QMAKE_EXTRA_TARGETS += first

SOURCES += \
    main.cpp\
    cuteseco.cpp \
    about/aboutdialog.cpp \
    logger/logger.cpp \
    logger/loggertypedelegate.cpp \
    config/config.cpp

HEADERS += \
    buildno.h \
    cuteseco.h \
    about/aboutdialog.h \
    logger/logger.h \
    logger/loggertypedelegate.h \
    logger/logger_include.h \
    config/config.h

FORMS += \
    cuteseco.ui \
    about/aboutdialog.ui

RESOURCES += \
    resource.qrc

CONFIG(debug, debug|release) {
message(+development mode)

#SOURCES += \

#HEADERS += \

#FORMS += \
}

CODECFORTR = UTF-8
CODECFORSRC = UTF-8

TRANSLATIONS = \
    LANG/lang_de.ts

OTHER_FILES += \
    buildno \
    CHANGELOG.txt \
    components.txt \
    LICENSE \
    LICENSE_HEADER \
    README.md \
    $${PROGNAME}.desktop \
    $${PROGNAME}.rc \
    $${PROGNAME}.sh \
    $${PROGNAME}Developer.desktop \
    $${PROGNAME}Developer.rc \
    $${PROGNAME}Developer.sh \
    SCRIPTS/increment-buildno.bat \
    SCRIPTS/increment-buildno
