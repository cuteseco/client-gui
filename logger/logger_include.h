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

#ifndef LOGGER_INCLUDE
#define LOGGER_INCLUDE

#define LOG_SECURITYERROR_COLOR      QColor(Qt::red)
#define LOG_ERROR_COLOR              QColor(Qt::red)
#define LOG_WARNING_COLOR            QColor(Qt::yellow)
#define LOG_SECURITYINFO_COLOR       QColor(Qt::green)
#define LOG_INFO_COLOR               QColor(Qt::green)
#define LOG_DEBUGINFO_COLOR          QColor(Qt::green)
#define LOG_DEBUGDETAILINFO_COLOR    QColor(Qt::green)

enum LOG_TYPES {
    LOG_SECURITYERROR,              /* security-related errors */
    LOG_ERROR,                      /* runtime errors */
    LOG_WARNING,                    /* this should not happen */
    LOG_SECURITYINFO,               /* security-related information */
    LOG_INFO,                       /* runtime information */
    LOG_DEBUGINFO,                  /* debug information */
    LOG_DEBUGDETAILINFO             /* very detailed debug information */
};
typedef enum LOG_TYPES LOG_TYPE;

Q_DECLARE_METATYPE(LOG_TYPE);

#endif // LOGGER_INCLUDE
