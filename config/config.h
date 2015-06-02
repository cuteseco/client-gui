/* Copyright (C) 2015  Christian Höne <christianhoene@cuteseco.org>
 *
 * This file is part of CuteSeCo.
 *
 * CuteSeCo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CuteSeCo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with CuteSeCo.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von CuteSeCo.
 *
 * CuteSeCo ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Affero General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
 * veröffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 * CuteSeCo wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU Affero General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
 * diesem Programm erhalten haben.
 * Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include <QStandardPaths>
#include <QtNetwork>
#include <QTranslator>

#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

//#include "buildno.h"
#include "logger/logger.h"

// proxy
#define SETTING_PROXYUSE                "proxyuse"
#define SETTINGSOPTION_NOPROXY          0
#define SETTINGSOPTION_MANUALPROXY      1
#define SETTINGSOPTION_AUTOPROXY        2
#define SETTING_PROXYURL                "proxyurl"
#define SETTING_PROXYUSER               "proxyuser"
#define SETTING_PROXYPASSWORD           "proxypassword"

// ui
#define SETTING_SAVESTATE_VERSION       1
#define SETTING_MAINSTATE               "mainstate"
#define SETTING_MAINGEOMETRY            "mainwindowgeometry"
#define SETTING_MAINPOSITION            "mainwindowsposition"

// OS
#if defined(Q_OS_LINUX)
    #define _OS_TYPE                    QString("GNU/Linux")
    #define _USERDEVICE                 QString("pc")
#elif defined(Q_OS_ANDROID)
    #define _OS_TYPE                    QString("Android")
    #define _USERDEVICE                 QString("mobile")
#elif defined(Q_OS_OSX)
    #define _OS_TYPE                    QString("OS_X")
    #define _USERDEVICE                 QString("mac")
#elif defined(Q_OS_IOS)
    #define _OS_TYPE                    QString("iOS")
    #define _USERDEVICE                 QString("mobile")
#elif defined(Q_OS_FREEBSD_KERNEL)
    #define _OS_TYPE                    QString("Free BSD")
    #define _USERDEVICE                 QString("pc")
#elif defined(Q_OS_WIN32)
    #define _OS_TYPE                    QString("Windows")
    #define _USERDEVICE                 QString("pc")
#elif defined(Q_OS_WINRT)
    #define _OS_TYPE                    QString("Windows RT")
    #define _USERDEVICE                 QString("mobile")
#elif defined(Q_OS_WINPHONE)
    #define _OS_TYPE                    QString("Windows Phone")
    #define _USERDEVICE                 QString("mobile")
#else
    #define _OS_TYPE                    QString("unknown")
    #define _USERDEVICE                 QString("unknown")
#endif

// ARCHITECTURE
#if defined(Q_PROCESSOR_X86_64)
    #define _ARCHITECTURE               QString("x86_64")
#elif defined(Q_PROCESSOR_X86_32)
    #define _ARCHITECTURE               QString("x86_32")
#endif


// shortcuts
#define KS_QUIT                         QKeySequence(Qt::CTRL + Qt::Key_Q)
#define KS_HELP                         QKeySequence::HelpContents
#define KS_SETTINGS                     QKeySequence(Qt::Key_F2)

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(QObject *parent = 0);
    ~Config();

    static bool             USE_TRAYICON;
    static int              LOG_LEVEL;
    static QTranslator      *TRANSLATOR_QT;
    static QTranslator      *TRANSLATOR_APP;

    // log
    static void log(QString logtext, LOG_TYPE type);

    static QString getFullProgName();

    // proxy
    QNetworkProxy loadProxy();
    void initProxy();
    void setProxy(QNetworkProxy proxy);
    static QNetworkProxy getProxy();
    static QString proxy2String(QNetworkProxy proxy);

    // path
    static QString getStoragePath();
    static QString getDownloadPath();
    static QString getDocumentPath();
    static QString getTempPath();
    static QString getCachePath();
    QString getSettingsPath();

    // host specific
    static QString getLocale();
    static QString getOSType();
    static QString getArchitecture();
    static QString getOSName();
    static bool isAdministrator();

signals:

public slots:

private:
    QSettings               settings;

    static QNetworkProxy    my_proxy;
};

#endif // CONFIG_H
