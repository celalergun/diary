#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    QString Password();
    QString Password2();
    QStringList Files();

    void ShowPasswordVerifier();

private slots:
    void on_btnSelectFiles_clicked();

    void on_chkUseKeyFiles_stateChanged(int arg1);

    void on_editPassword_2_textEdited(const QString &arg1);

private:
    Ui::LoginDialog *ui;
    QStringList m_FileList;
};

#endif // LOGINDIALOG_H
