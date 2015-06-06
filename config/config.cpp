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

#include "config.h"

#undef LOG_CLASSNAME
#define LOG_CLASSNAME QString("Config")

Q_GLOBAL_STATIC(Logger, _LOGGER)

QNetworkProxy Config::my_proxy;
bool Config::USE_TRAYICON=true;
QTranslator* Config::TRANSLATOR_QT=NULL;
QTranslator* Config::TRANSLATOR_APP=NULL;
QString Config::activeLanguage = "";

Config::Config(QObject *parent) :
    QObject(parent)
{
}

Config::~Config()
{
}

/*!
 * helper: log proxy
 * \param logtext
 * \param type
 */
void Config::log(QString logtext, LOG_TYPE type)
{
    LOG_DEFAULTLOGPROXY
}

QString Config::getFullProgName()
{
    LOG_CALL;

    return QString("%1 %2Build:%3 %4 %5 (%6)")
            .arg(PROJECT_PROGNAME)
            .arg(PROJECT_VERSION)
            .arg(PROJECT_BUILDNO)
            .arg(_ARCHITECTURE)
            .arg(_OS_TYPE)
            .arg(PROJECT_STATE);
}

QIcon Config::getProgIcon()
{
    LOG_CALL;

    return QIcon(QString(":/images/logos/%1_logo.svg").arg(PROJECT_PROGNAME));
}

/*!
 * Load language and install translation.
 * \param language
 */
void Config::loadLanguage(QString language)
{
    LOG_CALL;

    QLocale locale(language);
    QLocale::setDefault(locale);

    QString languageName = QLocale::languageToString(locale.language());
    bool    qtOK  = true,
            appOK = true;

    qApp->removeTranslator(Config::TRANSLATOR_QT);
    qApp->removeTranslator(Config::TRANSLATOR_APP);
    if (language != "en")
    {
        qtOK = Config::TRANSLATOR_QT->load(
                    "qt_"+language,
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)
                    );

        if (qtOK)
            qApp->installTranslator(Config::TRANSLATOR_QT);

        appOK = Config::TRANSLATOR_APP->load(":/translations/lang_"+language);

        if (appOK)
            qApp->installTranslator(Config::TRANSLATOR_APP);
    }

    activeLanguage = language;

    if (qtOK && appOK)
        log(QString("switched to language: %1").arg(languageName), LOG_INFO);
    else
        log(QString("error loading language: %1").arg(languageName), LOG_ERROR);
}

QString Config::getLanguage()
{
    LOG_CALL;

    return activeLanguage;
}

/*!
 * Reads configuration for proxy.
 * \return proxy
 */
QNetworkProxy Config::loadProxy()
{
    LOG_CALL;

    QNetworkProxy proxy;

    switch(settings.value(SETTING_PROXYUSE).toInt())
    {
    case SETTINGSOPTION_NOPROXY:
    {
        log("no proxy", LOG_DEBUGINFO);

        proxy.setType(QNetworkProxy::NoProxy);
    }; break;
    case SETTINGSOPTION_MANUALPROXY:
    {
        log("manual proxy", LOG_DEBUGINFO);

        QUrl proxyurl(settings.value(SETTING_PROXYURL).toString());
        if (proxyurl.isEmpty())
        {
            log("INVALID MANUAL PROXY. Fallback to default proxy.", LOG_ERROR);
            proxy.setType(QNetworkProxy::DefaultProxy);
        } else
        {
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(proxyurl.host());
            proxy.setPort(proxyurl.port());
            proxy.setUser(settings.value(SETTING_PROXYUSER, "").toString());
            proxy.setPassword(settings.value(SETTING_PROXYPASSWORD,
                                             ""
                                             ).toString()
                              );
        }
    }; break;
    case SETTINGSOPTION_AUTOPROXY:
    default:
    {
        log("default system proxy", LOG_DEBUGINFO);

        QNetworkProxyFactory::setUseSystemConfiguration(true);

        QList<QNetworkProxy> listOfProxies =
                QNetworkProxyFactory::systemProxyForQuery(
                    QNetworkProxyQuery(
                        QUrl(QLatin1String("https://cuteseco.org"))
                        )
                    );

        if (listOfProxies.isEmpty())
            proxy.setType(QNetworkProxy::NoProxy);
        else
            proxy = listOfProxies.at(0);
    }
    }

    return proxy;
}

/*!
 * Initializes proxy.
 */
void Config::initProxy()
{
    LOG_CALL;

    setProxy(loadProxy());
}

/*!
 * Installs proxy.
 * \param proxy
 */
void Config::setProxy(QNetworkProxy proxy)
{
    LOG_CALL;

    log(QString("proxy: %1").arg(proxy2String(proxy)), LOG_DEBUGINFO);
    my_proxy = proxy;
    QNetworkProxy::setApplicationProxy(my_proxy);
}

QNetworkProxy Config::getProxy()
{
    return my_proxy;
}

/*!
 * helper: human readable proxy configuration
 * \param proxy
 * \return configuration of proxy in a url
 */
QString Config::proxy2String(QNetworkProxy proxy)
{
    QString proxytype("");

    if (proxy.type() == QNetworkProxy::DefaultProxy)
    {
        proxytype = "(default) ";
        proxy = my_proxy;
    }

    switch (proxy.type())
    {
    case QNetworkProxy::NoProxy:
    {
        return "no proxy";
    }; break;
    case QNetworkProxy::Socks5Proxy:
    {
        proxytype += "socks";
    }; break;
    case QNetworkProxy::HttpProxy:
    {
        proxytype += "http";
    }; break;
    default: proxytype += "unknown";
    }

    QString credentials;

    if (!proxy.user().isEmpty() || !proxy.password().isEmpty())
    {
        if (!proxy.user().isEmpty())
            credentials.append("<user>:");

        if (!proxy.password().isEmpty())
            credentials.append("<password>");

        credentials.append("@");
    }

    return QString("%1://%2%3:%4")
            .arg(proxytype)
            .arg(credentials)
            .arg(proxy.hostName())
            .arg(proxy.port());
}

QString Config::getStoragePath()
{
    LOG_CALL;

    QString storagePath =
            QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(storagePath);
    if (!dir.exists())
        dir.mkpath(storagePath);
    return dir.path();
}

QString Config::getDownloadPath()
{
    LOG_CALL;

    QString downloadPath =
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QDir dir(downloadPath);
    if (!dir.exists())
        dir.mkpath(downloadPath);

    return dir.absolutePath()+"/";
}

QString Config::getDocumentPath()
{
    LOG_CALL;

    QString documentPath =
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir dir(documentPath);
    if (!dir.exists())
        dir.mkpath(documentPath);

    return dir.absolutePath()+"/";
}

QString Config::getTempPath()
{
    LOG_CALL;

    QString tempPath =
            QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(tempPath);
    if (!dir.exists())
        dir.mkpath(tempPath);

    return dir.absolutePath()+"/";
}

QString Config::getCachePath()
{
    LOG_CALL;

    QString cachePath =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(cachePath);
    if (!dir.exists())
        dir.mkpath(cachePath);
    return dir.path();
}

QString Config::getSettingsPath()
{
    LOG_CALL;

    return settings.fileName();
}

QString Config::getSystemLocale()
{
    LOG_CALL;

    QString defaultLocale = QLocale::system().name();
    int pos = defaultLocale.lastIndexOf('_');
    if (pos != -1)
        defaultLocale.truncate(pos); // "de_DE" -> "de"
    return defaultLocale;
}

QString Config::getOSType()
{
    LOG_CALL;

    return _OS_TYPE;
}

QString Config::getArchitecture()
{
    LOG_CALL;

    return _ARCHITECTURE;
}

QString Config::getOSName()
{
    LOG_CALL;

    QString OS = "not supported";

#if defined(Q_OS_LINUX)
    OS = "Linux";

    QString programm = "/usr/bin/lsb_release";
    if (QFileInfo(programm).exists())
    {
        QStringList programmArgs;
        programmArgs << "-sd";
        QProcess *linuxOS = new QProcess();
        linuxOS->start(programm, programmArgs);
        linuxOS->waitForFinished();
        OS = linuxOS->readLine(1024);
        OS.replace("\"", "");
    } else
    {
        QString fileName = "/etc/SuSE-release";
        if (QFileInfo(fileName).exists())
        {
            QFile file(fileName);
            if (file.open(QFile::ReadOnly))
                OS = file.readLine(1024);
        }
    }

#elif defined(Q_OS_WIN32)
    OS = "unsupported Windows";
    // msdn: using system information
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    if (GetVersionEx((OSVERSIONINFO *)&osvi))
    {
        bool workstation = osvi.wProductType == VER_NT_WORKSTATION;
        SYSTEM_INFO si;
        ZeroMemory(&si, sizeof(SYSTEM_INFO));
        typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
        if (PGNSI pGNSI =
                PGNSI(QLibrary("kernel32").resolve("GetNativeSystemInfo")))
            pGNSI(&si);
        else
            GetSystemInfo(&si);
        QString os;
        switch ((osvi.dwMajorVersion << 8) + osvi.dwMinorVersion)
        {
        case 0x0500: os = workstation ? "2000 Professional" : "2000 Server";
            break;
        case 0x0501: os = osvi.wSuiteMask & VER_SUITE_PERSONAL ?
                        "XP Home" : "XP Professional";
            break;
        case 0x0502:
            if (GetSystemMetrics(SM_SERVERR2))
                os = "Server 2003 R2";
            else if (workstation &&
                     si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                os = "XP Professional";
            else
                os = "Server 2003";
            break;
        case 0x0600: os = workstation ? "Vista" : "Server 2008";
            break;
        case 0x0601: os = workstation ? "7" : "Server 2008 R2";
            break;
        case 0x0602: os = workstation ? "8" : "Server 2012";
            break;
        case 0x0603: os = workstation ? "8.1" : "Server 2012 R2";
            break;
        default: os = "unknown";
            break;
        }
        QString extversion((const QChar*)osvi.szCSDVersion);
        return QString("Windows %1 %2(%3 bit)").arg(os)
                .arg(extversion.isEmpty() ? "" : extversion + " ")
                .arg(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ?
                         "64" : "32");
    } else
    {
        switch( QSysInfo::WindowsVersion )
        {
        case QSysInfo::WV_2000:         OS = "Windows 2000 ";
            break;
        case QSysInfo::WV_XP:           OS = "Windows XP ";
            break;
        case QSysInfo::WV_2003:         OS = "Windows 2003 ";
            break;
        case QSysInfo::WV_VISTA:        OS = "Windows Vista ";
            break;
        case QSysInfo::WV_WINDOWS7:     OS = "Windows 7 ";
            break;
        case QSysInfo::WV_WINDOWS8:     OS = "Windows 8 ";
            break;
        case QSysInfo::WV_WINDOWS8_1:   OS = "Windows 8.1 ";
            break;
        default: OS = "Windows unknown ";
            break;
        }
        OS += getArchitecture();
    }
#elif defined(Q_OS_OSX)
    OS = "Mac OS";
    QFile f("/System/Library/CoreServices/SystemVersion.plist");
    if (f.open(QFile::ReadOnly))
    {
        QXmlStreamReader xml(&f);
        while (xml.readNext() != QXmlStreamReader::Invalid)
        {
            if (!xml.isStartElement()
                    || xml.name() != "key"
                    || xml.readElementText() != "ProductVersion"
                    )
                continue;
            xml.readNextStartElement();
            OS = QString("Mac OS %1").arg(xml.readElementText());
        }
    }
#endif

    OS.replace("\n", "");
    return OS;
}

bool Config::isAdministrator()
{
    LOG_CALL;

#if defined(Q_OS_LINUX)
    return (getuid() ? false : true);
#elif defined(Q_OS_WIN32)
    BOOL b;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    b = AllocateAndInitializeSid(
                &NtAuthority,
                2,
                SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS,
                0, 0, 0, 0, 0, 0,
                &AdministratorsGroup);
    if (b)
    {
        if (!CheckTokenMembership(NULL, AdministratorsGroup, &b))
        {
            b = FALSE;
        }
        FreeSid(AdministratorsGroup);
    }

    return(b);
#endif
}
