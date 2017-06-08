#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>

QString testEncrypt()
{
    AesClass enc;
    QByteArray key = QString("1234567890123456").toUtf8();
    QString test;
    for (int i = 0; i < 100; ++i) {
        test = test + "Pijamalı hasta yağız şoföre çabucak güvendi ";
    }
    QByteArray arr = enc.Encrypt(test.toUtf8(), key);
    QByteArray res = enc.Decrypt(arr, key);
    test = QString::fromUtf8(res);
    return test;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->menuEntry->setEnabled(false);
    m_Dirty = false;

    //setWindowTitle(testEncrypt());

    DbOp = DatabaseOperations();
    if (!DbOp.Connect("diary.db3"))
    {
        ui->textEdit->setReadOnly(true);
        ui->menuDiary->setEnabled(false);
        ui->menuEntry->setEnabled(false);
        QMessageBox::critical(this, "Error", "Couldn't create database", QMessageBox::Ok);
    }
    if (!DbOp.IsPreferencesSet())
    {
        if (QMessageBox::Yes ==
                QMessageBox::question(
                    this,
                    "Password usage",
                    "It seems like you have just installed this application. You can use a password to login when you start it. Do you want to use a password?",
                    QMessageBox::Yes | QMessageBox::No))
        {
            LoginDialog *login = new LoginDialog(this);
            login->ShowPasswordVerifier();
            login->move(QApplication::desktop()->screen()->rect().center() - login->rect().center());
            int res = login->exec();
            if (res == QDialog::Rejected)
                exit(0);

            QString pass1 = login->Password();
            QString pass2 = login->Password2();
            if (pass1 != pass2)
            {
                QMessageBox::critical(this, "Error", "Passwords do not match. Cannot set a password!", QMessageBox::Ok);
                exit(0);
            }
            else
            {
                DbOp.SetEncryption(true, pass1, login->Files());
            }
            delete login;
        }
        else
        {
            DbOp.SetEncryption(false, "", QStringList());
        }
        DbOp.ReadPreferences();
    }
    else
    {
        DbOp.ReadPreferences();
        if (DbOp.IsPasswordEnabled())
        {
            LoginDialog *login = new LoginDialog(this);
            login->move(QApplication::desktop()->screen()->rect().center() - login->rect().center());
            int res = login->exec();
            if ((res == QDialog::Rejected))
                exit(0);
            if (!DbOp.IsPasswordValid(login->Password(), login->Files()))
                exit(0);

            delete login;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)  // show prompt when user wants to close app
{
    if (m_Dirty)
    {
        event->ignore();
        if (QMessageBox::Yes == QMessageBox::question(this, "Exit?", "You changed your text and not saved it yet. Are you sure?", QMessageBox::Yes | QMessageBox::No))
        {
            event->accept();
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_textEdit_textChanged()
{
    if (!m_Dirty)
        ui->statusBar->showMessage("* Modified");

    m_Dirty = true;
}

void MainWindow::DisplayRecord(int rowId)
{
    QString entry = DbOp.RetrieveEntryFromDatabase(rowId);
    QString title = DbOp.CurrentTitle() + " - " + DbOp.CurrentTimestamp();
    this->setWindowTitle(title);
    ui->textEdit->setPlainText(entry);
    ui->textEdit->setFocus();
    ui->textEdit->setReadOnly(false);
    ui->menuEntry->setEnabled(true);
    m_Dirty = false;
}

void MainWindow::on_actionNew_entry_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Title of the day",
                                         "What was it all about today?", QLineEdit::Normal,
                                         "Subject", &ok);
    if (ok && !text.isEmpty())
    {

        int rowId = DbOp.AddNewEntryToDatabase(text);
        if (rowId == -1)
            return;

        DisplayRecord(rowId);
        ui->statusBar->showMessage("New entry created...");
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString entry = ui->textEdit->toPlainText();
    if (!DbOp.SaveEntryToDatabase(entry))
    {
        QMessageBox(QMessageBox::Critical, "Error", "Cannot save entry to the database: ", QMessageBox::Ok).exec();
        return;
    }
    m_Dirty = false;
    ui->statusBar->showMessage("Saved.");
}

void MainWindow::on_actionDelete_triggered()
{
    if (!DbOp.DeleteEntryFromDatabase(DbOp.m_Id))
    {
        QMessageBox(QMessageBox::Critical, "Error", "Cannot delete entry from the database: ", QMessageBox::Ok).exec();
        return;
    }
    ui->textEdit->clear();
    ui->textEdit->setReadOnly(true);
    m_Dirty = false;
    ui->statusBar->showMessage("Deleted!");
}

void MainWindow::on_actionSelect_a_previous_entry_to_read_edit_triggered()
{
    EntryList* el = new EntryList(this);

    auto list = DbOp.RetrieveEntryList(QDateTime::fromString("2017-01-01", "yyyy-MM-dd"));

    for (int i = 0; i<list.count();i++)
    {
        el->Add(list[i].Id, list[i].Title, list[i].TimeStamp.toString("yyyy-MM-dd hh:mm:ss"));
    }

    int res = el->exec();
    int rowId = el->SelectedId();
    int rowNum = el->SelectedRow();
    delete el;

    if ((res == QDialog::Rejected) || (rowNum < 0))
        return;

    DisplayRecord(rowId);
    ui->statusBar->showMessage("Loaded...");
}
