#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->menuEntry->setEnabled(false);
    m_Dirty = false;
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
        if (QMessageBox::Yes == QMessageBox::question(this, "Password usage", "It seems like you have just installed this application. You can use a password to login when you start it. Do you want to use a password?", QMessageBox::Yes | QMessageBox::No))
        {
            bool ok;
            QString pass1 = QInputDialog::getText(this, "Password",
                                                 "Please enter your password", QLineEdit::Password,
                                                 "", &ok);
            QString pass2 = QInputDialog::getText(this, "Password",
                                                 "Please enter your password again", QLineEdit::Password,
                                                 "", &ok);
            if (pass1 != pass2)
            {
                QMessageBox::critical(this, "Error", "Passwords do not match. Cannot set a password!", QMessageBox::Ok);
                return;
            }
            else
            {
                DbOp.SetEncryption(true, pass1);
            }
        }
    }
//    QByteArray key = QString("123456789").toUtf8();
//    QString test = "Pijamalı hasta yağız şoföre çabucak güvendi";
//    QByteArray arr = DatabaseOperations::EncryptString(test, key);
//    test = DatabaseOperations::DecryptString(arr, key);
//    setWindowTitle(test);
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
        ui->statusBar->setWindowTitle("* Modified");

    m_Dirty = true;
}

void MainWindow::DisplayRecord(int rowId)
{
    QString entry = DbOp.RetrieveEntryFromDatabase(rowId);
    QString title = DbOp.m_Title + " - " + DbOp.m_TimeStamp;
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
                                         "What is it all about today?", QLineEdit::Normal,
                                         "Subject", &ok);
    if (ok && !text.isEmpty())
    {

        int rowId = DbOp.AddNewEntryToDatabase(text);
        if (rowId == -1)
            return;

        DisplayRecord(rowId);
        ui->statusBar->setWindowTitle("New entry created...");
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString entry = ui->textEdit->toPlainText();
    if (!DbOp.SaveToDatabase(entry))
    {
        QMessageBox(QMessageBox::Critical, "Error", "Cannot save entry to the database: ", QMessageBox::Ok).exec();
        return;
    }
    m_Dirty = false;
    ui->statusBar->setWindowTitle("Saved.");
}

void MainWindow::on_actionDelete_triggered()
{
    if (!DbOp.DeleteFromDatabase(DbOp.m_Id))
    {
        QMessageBox(QMessageBox::Critical, "Error", "Cannot delete entry from the database: ", QMessageBox::Ok).exec();
        return;
    }
    ui->textEdit->clear();
    ui->textEdit->setReadOnly(true);
    m_Dirty = false;
    ui->statusBar->setWindowTitle("Deleted!");
}

void MainWindow::on_actionSelect_a_previous_entry_to_read_edit_triggered()
{
    EntryList* el = new EntryList(this);

    auto list = DbOp.RetrieveList(QDateTime::fromString("2017-01-01", "yyyy-MM-dd"));

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
    ui->statusBar->setWindowTitle("Loaded...");
}
