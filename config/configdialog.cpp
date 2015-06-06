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

#include "configdialog.h"
#include "ui_configdialog.h"

Q_GLOBAL_STATIC(Logger, _LOGGER)

/*!
 * Constructor of ConfigDialog.
 * \param parent
 */
ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    initProxy();
    on_checkB_show_password_toggled(false);
}

/*!
 * Destructor of ConfigDialog.
 */
ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QDialog::changeEvent(event);
}

/*!
 * helper: log proxy
 * \param logtext
 * \param type
 */
void ConfigDialog::log(QString logtext, LOG_TYPE type)
{
    LOG_DEFAULTLOGPROXY
}

/*!
 * Reads configuration for proxy.
 */
void ConfigDialog::initProxy()
{
    LOG_CALL;

    switch(settings.value(SETTING_PROXYUSE).toInt())
    {
    case SETTINGSOPTION_NOPROXY:
    {
        ui->rB_use_no_proxy->setChecked(true);
    }; break;
    case SETTINGSOPTION_MANUALPROXY:
    {
        ui->rB_use_own_proxy->setChecked(true);
    }; break;
    case SETTINGSOPTION_AUTOPROXY:
    default:
    {
        ui->rB_use_system_proxy->setChecked(true);
    }; break;
    }

    ui->lE_proxy->setText(settings.value(SETTING_PROXYURL,
                                         "http://proxy.example.com:3128"
                                         ).toString());
    ui->lE_proxy_user->setText(settings.value(SETTING_PROXYUSER,
                                              ""
                                              ).toString());
    ui->lE_proxy_password->setText(settings.value(SETTING_PROXYPASSWORD,
                                                  ""
                                                  ).toString());
}

/*!
 * Saves changes are made.
 */
void ConfigDialog::on_buttonBox_accepted()
{
    LOG_CALL;

    // proxy
    int proxyuse;
    if (ui->rB_use_no_proxy->isChecked() == true)
        proxyuse = SETTINGSOPTION_NOPROXY;
    if (ui->rB_use_own_proxy->isChecked() == true)
        proxyuse = SETTINGSOPTION_MANUALPROXY;
    if (ui->rB_use_system_proxy->isChecked() == true)
        proxyuse = SETTINGSOPTION_AUTOPROXY;

    settings.setValue(SETTING_PROXYUSE, proxyuse);
    settings.setValue(SETTING_PROXYURL, ui->lE_proxy->text());
    settings.setValue(SETTING_PROXYUSER, ui->lE_proxy_user->text());
    settings.setValue(SETTING_PROXYPASSWORD, ui->lE_proxy_password->text());
    initProxy();

    Config().initProxy();
}

/*!
 * Revokes changes are made.
 */
void ConfigDialog::on_buttonBox_rejected()
{
    LOG_CALL;

    initProxy();
}

/*!
 * Toggles between plain and password/star view.
 * \param checked
 */
void ConfigDialog::on_checkB_show_password_toggled(bool checked)
{
    LOG_CALL;

    ui->lE_proxy_password->setEchoMode(checked ?
                                           QLineEdit::Normal :
                                           QLineEdit::Password);
}
