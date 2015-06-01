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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("About %1").arg(PROJECT_PROGNAME));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->l_version->setText(Config::getFullProgName());
    ui->tBrowser_about->viewport()->setAutoFillBackground(false);
    ui->tBrowser_license->viewport()->setAutoFillBackground(false);

    loadingHtmlContent();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        loadingHtmlContent();
    }
    QDialog::changeEvent(event);
}

/* loading html content
 */
void AboutDialog::loadingHtmlContent()
{
    QString fileName(":/about/about_de.html");

    QFileInfo fileInfo(fileName);

    if (!fileInfo.exists())
        fileName = ":/about/about_en.html";

    QFile about_file(fileName);
    if (about_file.open(QFile::ReadOnly|QFile::Text))
    {
        QTextStream istream(&about_file);
        istream.setCodec("UTF-8");
        ui->tBrowser_about->setHtml(istream.readAll());
        about_file.close();
    }

    QFile license_file(":/LICENSE");
    if (license_file.open(QFile::ReadOnly|QFile::Text))
    {
        QTextStream istream(&license_file);
        istream.setCodec("UTF-8");
        ui->tBrowser_license->setText(istream.readAll());
        license_file.close();
    }

}

void AboutDialog::on_pB_qt_clicked()
{
    QMessageBox::aboutQt(this, tr("%1 uses Qt").arg(PROJECT_PROGNAME));
}
