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

#include "cuteseco.h"
#include <QApplication>

#include "config/config.h"
#include "logger/logger.h"

Q_GLOBAL_STATIC(Logger, _LOGGER)

void log(QString logtext, LOG_TYPE type)
{
    if (GLOBAL_logger)
        GLOBAL_logger->append(logtext, type);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Christian Höne");
    QCoreApplication::setOrganizationDomain("cuteseco.org");
    QCoreApplication::setApplicationName(PROJECT_PROGNAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QApplication a(argc, argv);

    log(QString("%1 is starting.").arg(PROJECT_PROGNAME), LOG_INFO);

    // logging and log level
#if defined(PROJECT_DEVELOPERMODE)
    GLOBAL_logger->setLogLevel(LOG_DEBUGDETAILINFO);
#else
    GLOBAL_logger->setLogLevel(LOG_ERROR);
#endif

    // set up translation
    QTranslator qtTranslator, appTranslator;
    Config::TRANSLATOR_QT = &qtTranslator;
    Config::TRANSLATOR_APP = &appTranslator;

    Config::loadLanguage(Config::getSystemLocale());

    a.installTranslator(&qtTranslator);
    a.installTranslator(&appTranslator);


    CuteSeCo w;
    w.show();

    int returncode = a.exec();
    log(QString("%1 quits. (%2)").arg(PROJECT_PROGNAME).arg(returncode),
        LOG_INFO
        );
    return returncode;
}
