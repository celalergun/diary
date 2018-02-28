#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QCloseEvent>

#include "databaseoperations.h"
#include "entrylist.h"
#include "aesclass.h"
#include "logindialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool baglantiOlustur(QString DosyaAdi);

    void DisplayRecord(int rowId);

    void closeEvent(QCloseEvent *event);  // show prompt when user wants to close app

private slots:
    void on_actionQuit_triggered();

    void on_textEdit_textChanged();

    void on_actionNew_entry_triggered();

    void on_actionSave_triggered();

    void on_actionDelete_triggered();

    void on_actionSelect_a_previous_entry_to_read_edit_triggered();

private:
    Ui::MainWindow *ui;
    bool m_Dirty = false;
    DatabaseOperations DbOp;
};

#endif // MAINWINDOW_H
