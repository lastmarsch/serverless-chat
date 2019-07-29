#include "pm.h"
#include "ui_pm.h"

PM::PM(QWidget *parent, QString destination_name, Chat* chat, peer* user) :
    QDialog(parent),
    ui(new Ui::PM),
    talker(destination_name)
{
    ui->setupUi(this);
    ch = chat;
    us = user;

    ui->chatArea->setReadOnly(true);
    ui->textArea->setFocus();
    ui->chatName->setText("Chat with: " + talker);

    connect(this, SIGNAL(text_sent_pm(QString, QString, QString, QHostAddress)),
            chat, SLOT(send(QString, QString, QString, QHostAddress)));
    connect(chat, SIGNAL(data_received_pm(QString, QString, QString)),
            this, SLOT(set_received_text_pm(QString, QString, QString)));
    connect(this, SIGNAL(pm_closed(QString)), parent, SLOT(closed_pm_process(QString)));

}

PM::PM(QWidget *parent, QString destination_name, QString message, Chat* chat, peer* user) :
    QDialog(parent),
    ui(new Ui::PM),
    talker(destination_name)
{
    ui->setupUi(this);
    ch = chat;
    us = user;

    ui->chatArea->setReadOnly(true);
    ui->textArea->setFocus();
    ui->chatName->setText("Chat with: " + talker);

    ui->chatArea->append(message);

    connect(this, SIGNAL(text_sent_pm(QString, QString, QString, QHostAddress)),
            chat, SLOT(send(QString, QString, QString, QHostAddress)));
    connect(chat, SIGNAL(data_received_pm(QString, QString, QString)),
            this, SLOT(set_received_text_pm(QString, QString, QString)));
    connect(this, SIGNAL(pm_closed(QString)), parent, SLOT(closed_pm_process(QString)));

}

PM::~PM()
{
    delete ui;
}

void PM::reject()
{
    emit pm_closed(talker);
    QDialog::reject();
}

void PM::on_sendButton_clicked()
{
    QString text = ch->setData()+" "+us->nickname+":\n"+ui->textArea->text();

    ui->textArea->clear();

    QHostAddress to;

    for(int i=0; i < ch->peers.count(); i++)
    {
        if(ch->peers[i].nickname == talker)
        {
            to = ch->peers[i].host;
            break;
        }
    }
    emit text_sent_pm("PM", talker, text, QHostAddress::Broadcast);
}

void PM::set_received_text_pm(QString sender, QString receiver, QString data)
{
    if ((sender == talker || sender == us->nickname)&&(receiver == talker || receiver == us->nickname))
    {
        ui->chatArea->append(data);
    }
}
