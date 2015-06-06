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

/* singleton Logger
 */

#include "logger.h"

Q_GLOBAL_STATIC(Logger, _LOGGER)

/*!
 * Constructor of Logger class.
 * \param parent
 */
Logger::Logger(QObject *parent) :
    QObject(parent)
{
    initialized = false;
    log_level = LOG_DEBUGDETAILINFO;
}

/*!
 * \return Pointer to "singleton-class" Logger
 */
Logger* Logger::globalInstance()
{
    return _LOGGER;
}

/*!
 * Destructor of Logger class.
 */
Logger::~Logger()
{
}

/*!
 * helper: holds text for LOG_TYPE
 * \param type
 * \return Human readable string of LOG_TYPE
 */
QString Logger::type2string(LOG_TYPE type)
{
    switch(type)
    {
    case LOG_DEBUGDETAILINFO:return tr("");
    case LOG_DEBUGINFO:      return tr("DEBUG");
    case LOG_INFO:           return tr("Info");
    case LOG_WARNING:        return tr("Warning");
    case LOG_ERROR:          return tr("Error");
    case LOG_SECURITYINFO:   return tr("Securityinfo");
    case LOG_SECURITYERROR:  return tr("SECURITY!");
    }
    return "%%%%%";
}

/*!
 * helper: holds description for LOG_TYPE
 * \param type
 * \return Description of LOG_TYPE
 */
QString Logger::type2description(LOG_TYPE type)
{
    switch(type)
    {
    case LOG_DEBUGDETAILINFO:return tr("debug detail info");
    case LOG_DEBUGINFO:      return tr("debug info");
    case LOG_INFO:           return tr("info");
    case LOG_WARNING:        return tr("warning");
    case LOG_ERROR:          return tr("error");
    case LOG_SECURITYINFO:   return tr("security info");
    case LOG_SECURITYERROR:  return tr("security alert");
    }
    return "%%%%%";
}

/*!
 * Initializes logger class.
 * connects signals and slots
 */
void Logger::init()
{
    if (GLOBAL_logger->isInitialized() || initialized)
        return;

    connect(GLOBAL_logger,
            SIGNAL(logoutput(QDateTime,QString,QString,LOG_TYPE)),
            GLOBAL_logger,
            SLOT(internalAdd(QDateTime,QString,QString,LOG_TYPE))
            );

    consoleLog(QDateTime::currentDateTime(),
               LOG_CLASSNAME,
               "--- Log system started ---",
               LOG_INFO
               );

    initialized = true;
}

/*!
 * \return State of initialization.
 */
bool Logger::isInitialized()
{
    return initialized;
}

/*!
 * Sets the log level to be displayed permanently.
 * \param level
 */
void Logger::setLogLevel(LOG_TYPE level)
{
    if (!initialized)
        init();

    log_level = level;

    QString listLoglevel;
    for (int i=0; i <= log_level; i++)
        listLoglevel.append(QString("\t\t %1 : (%2) %3\n")
                            .arg(type2string((LOG_TYPE)i),
                                 -LOG_TYPENAME_LENGTH,
                                 '-'
                                 )
                            .arg(i)
                            .arg(type2description((LOG_TYPE)i)));

    consoleLog(QDateTime::currentDateTime(),
               LOG_CLASSNAME,
               QString("log level is set to: (%1)\n%2")
               .arg(log_level)
               .arg("\n"+listLoglevel),
               LOG_INFO
               );
}

/*!
 * \return Current log level.
 */
LOG_TYPE Logger::getLogLevel()
{
    return log_level;
}

/*!
 * public: append log entry
 * log entry is queued through qt signal pipeline
 * \param classname
 * \param logtext
 * \param type
 */
void Logger::append(QString classname, QString logtext, LOG_TYPE type)
{
    if (!initialized)
        init();

    emit logoutput(QDateTime::currentDateTime(),classname,logtext,type);
}

/*!
 * private: processes log entry
 * \param time
 * \param classname
 * \param logtext
 * \param type
 */
void Logger::internalAdd(QDateTime time,
                         QString classname,
                         QString logtext,
                         LOG_TYPE type
                         )
{
    if (!initialized)
        init();

    if (type > log_level)
        return;

    consoleLog(time, classname, logtext, type);
}

/*!
 * console output of a log entry
 * \param time
 * \param classname
 * \param logtext
 * \param type
 */
void Logger::consoleLog(QDateTime time,
                        QString classname,
                        QString logtext,
                        LOG_TYPE type
                        )
{
    QString logline = QString("%1 %2 %3 %4")
            .arg(time.toString("dd.MM.yyyy HH:mm:ss.zzz"))
            .arg(type2string(type), -LOG_TYPENAME_LENGTH, '-')
            .arg(classname, -LOG_CLASSNAME_LENGTH)
            .arg(logtext);

    switch(type)
    {
    case LOG_SECURITYERROR:
    case LOG_ERROR:
    case LOG_WARNING:
        QTextStream(stderr) << logline << endl;
        break;

    case LOG_SECURITYINFO:
    case LOG_INFO:
    case LOG_DEBUGINFO:
    case LOG_DEBUGDETAILINFO:
        QTextStream(stdout) << logline << endl;
        break;
    }
}
