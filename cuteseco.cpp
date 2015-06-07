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
#include "ui_cuteseco.h"

Q_GLOBAL_STATIC(Logger, _LOGGER)

/*!
 * Constructor of CuteSeCo main class.
 * \param parent
 */
CuteSeCo::CuteSeCo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CuteSeCo)
{
    ui->setupUi(this);

    aboutDialog = new AboutDialog();
    configDialog = new ConfigDialog();

    setWindowIcon(Config::getProgIcon());

    // actions menu
    ui->action_Settings->setShortcut(KS_SETTINGS);
    ui->action_Quit->setShortcut(KS_QUIT);

    // language menu
    createLanguageMenu();

    // help menu
    ui->action_About->setShortcut(KS_HELP);
}

/*!
 * Destructor of CuteSeCo main class.
 */
CuteSeCo::~CuteSeCo()
{
    delete ui;
}

void CuteSeCo::changeEvent(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::LocaleChange:
        Config::loadLanguage(QLocale::system().name());
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

/*!
 * helper: log proxy
 * \param logtext
 * \param type
 */
void CuteSeCo::log(QString logtext, LOG_TYPE type)
{
    LOG_DEFAULTLOGPROXY
}

void CuteSeCo::on_action_Settings_triggered()
{
    LOG_CALL;

    configDialog->show();
}

void CuteSeCo::on_action_Quit_triggered()
{
    LOG_CALL;

    qApp->quit();
}

void CuteSeCo::on_action_About_triggered()
{
    LOG_CALL;

    aboutDialog->show();
}

/*!
 * Searches for existing language files and builds a menu for language choosing.
 */
void CuteSeCo::createLanguageMenu()
{
    LOG_CALL;

    QActionGroup *languageSelectionGroup = new QActionGroup(ui->menu_Language);
    languageSelectionGroup->setExclusive(true);

    connect(languageSelectionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(languageChangedEvent(QAction*)));

    QString defaultLocale = Config::getSystemLocale();

    QDir translationDir(":/translations");
    QStringList fileNames = translationDir.entryList(QStringList("*.qm"),
                                                     QDir::Files,
                                                     QDir::Name
                                                     );

    for (int i=0; i < fileNames.size(); i++)
    {
        QString locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_')+1);

        QIcon icon(":/images/flags/flag_"+locale+".png");

        QAction *action = new QAction(
                    icon,
                    QLocale::languageToString(QLocale(locale).language()),
                    this
                    );
        action->setCheckable(true);
        action->setData(locale);

        ui->menu_Language->addAction(action);
        languageSelectionGroup->addAction(action);
        if (locale == defaultLocale)
            action->setChecked(true);
    }
}

/*!
 * SLOT: triggered on language change
 * \param action
 */
void CuteSeCo::languageChangedEvent(QAction *action)
{
    LOG_CALL;

    Config::loadLanguage(action->data().toString());
}

