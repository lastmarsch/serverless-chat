#include "enterchatdialog.h"
#include "ui_enterchatdialog.h"

EnterChatDialog::EnterChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterChatDialog)
{
    ui->setupUi(this);
    ui->setNick->setFocus();
    setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );
}

EnterChatDialog::~EnterChatDialog()
{
    delete ui;
}

void EnterChatDialog::on_loginButton_clicked()
{
    QString nickname = ui->setNick->text();
    emit set_nickname(nickname);
}

void EnterChatDialog::warning_message(QString warning)
{
    ui->warningText->setText("<FONT COLOR=#800000>"+warning+"<FONT>");
}

void EnterChatDialog::reject()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::question( this,  QApplication::applicationName(), tr("Do you want to exit?"),
                                 QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
    if (ret == QMessageBox::No)
        ui->warningText->setText("<FONT COLOR=#800000>Please, log in first.<FONT>");
    else
    {
        emit rejected_enterdialog();
        QDialog::reject();
    }
}

void EnterChatDialog::end_work()
{
    this->deleteLater();
}
