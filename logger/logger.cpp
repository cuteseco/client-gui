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

#undef _MYNAME
#define _MYNAME QString("Logger:: ")

Logger::Logger(QObject *parent) :
    QObject(parent)
{
    loggerTypeDelegate = NULL;
    initialized = false;
    verbose_level = LOG_DEBUGDETAILINFO;
}

Logger* Logger::globalInstance()
{
    return _LOGGER;
}

Logger::~Logger()
{
}

void Logger::init()
{
    if (initialized)
        return;

    initialized = true;

    connect(_LOGGER->globalInstance(), SIGNAL(logoutput(QString,LOG_TYPE)),
            _LOGGER->globalInstance(), SLOT(addLog(QString,LOG_TYPE)));
}

void Logger::setVerboseLevel(int level)
{
    verbose_level = level;
}

void Logger::add(QString logtext, LOG_TYPE type)
{
    if (!initialized)
        init();

    emit logoutput(logtext, type);
}

void Logger::addLog(QString logtext, LOG_TYPE type)
{
    if (!initialized)
        init();

    if (type > verbose_level)
        return;

    QTextStream(stderr)
            << QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")
            << " "
            << LoggerTypeDelegate::type2string(type)
            << " "
            << logtext
            << endl;
}
