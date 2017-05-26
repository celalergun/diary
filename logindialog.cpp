#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->chkUseKeyFiles->setChecked(false);
    ui->btnSelectFiles->setVisible(false);
    ui->lblAgain->setVisible(false);
    ui->editPassword_2->setVisible(false);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::Password()
{
    return ui->editPassword->text();
}

QString LoginDialog::Password2()
{
    return ui->editPassword_2->text();
}

QStringList LoginDialog::Files()
{
    return m_FileList;
}

void LoginDialog::on_btnSelectFiles_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,tr("All files"),QDir::currentPath(),tr("All files (*.*);") );
    if( !filenames.isEmpty() )
    {
        for (int i =0;i<filenames.count();i++)
            m_FileList.append(filenames.at(i));
    }
}

void LoginDialog::on_chkUseKeyFiles_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->btnSelectFiles->setVisible(ui->chkUseKeyFiles->isChecked());
}

void LoginDialog::ShowPasswordVerifier()
{
    ui->lblAgain->setVisible(true);
    ui->editPassword_2->setVisible(true);
    ui->buttonBox->setEnabled(false);
}

void LoginDialog::on_editPassword_2_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->buttonBox->setEnabled((ui->editPassword->text() == ui->editPassword_2->text()));
}
