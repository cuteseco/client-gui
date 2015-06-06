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

#include "loggertypedelegate.h"

/*!
 * Constructor of LoggerTypeDelegate.
 * \param parent
 */
LoggerTypeDelegate::LoggerTypeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

/*!
 * Destructor of LoggerTypeDelegate.
 */
LoggerTypeDelegate::~LoggerTypeDelegate()
{
}

/*!
 * Paint method for item view.
 * \param painter
 * \param option
 * \param index
 */
void LoggerTypeDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index
                               ) const
{
    if (index.column() == 2)
    {
        painter->save();
        switch(index.data().toInt())
        {
        case LOG_DEBUGDETAILINFO:
        {
            painter->setPen(LOG_DEBUGDETAILINFO_COLOR);
        }; break;
        case LOG_DEBUGINFO:
        {
            painter->setPen(LOG_DEBUGINFO_COLOR);
        }; break;
        case LOG_INFO:
        {
            painter->setPen(LOG_INFO_COLOR);
        }; break;
        case LOG_WARNING:
        {
            painter->setPen(LOG_WARNING_COLOR);
        }; break;
        case LOG_ERROR:
        {
            painter->setPen(LOG_ERROR_COLOR);
        }; break;
        case LOG_SECURITYINFO:
        {
            painter->setPen(LOG_SECURITYINFO_COLOR);
        }; break;
        case LOG_SECURITYERROR:
        {
            painter->setPen(LOG_SECURITYERROR_COLOR);
        }; break;
        }

        painter->drawText(option.rect,
                          option.displayAlignment,
                          Logger::type2string((LOG_TYPE)index.data().toInt())
                          );
        painter->restore();
    } else
        QStyledItemDelegate::paint(painter, option, index);
}
