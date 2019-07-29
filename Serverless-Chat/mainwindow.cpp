#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->chatArea->setReadOnly(true);
    ui->textArea->setFocus();
    ui->sendButton->setShortcut(Qt::Key_Return);

    user = new peer();
    chat = new Chat(this, user);

    connect(enterCh, SIGNAL(set_nickname(QString)), this, SLOT(natural_selection(QString)));
    connect(this,  SIGNAL(type_another_nick(QString)), enterCh, SLOT(warning_message(QString)));
    connect(this,  SIGNAL(close_enter_dialog()), enterCh, SLOT(end_work()));
    connect(enterCh,  SIGNAL(rejected_enterdialog()), this, SLOT(close()));
    connect(ui->onlineList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(nick_double_clicked_process(QListWidgetItem*)));

    enterCh->setModal(true);
    enterCh->setWindowTitle("Enter chat");
    enterCh->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::natural_selection(QString nickname)
{
    if (nickname_verification(nickname))
    {
        user->nickname = nickname;
        chat->setName(nickname);

        emit close_enter_dialog();

        ui->welcomeText->setText("Welcome home, " + nickname);
    }
}

bool MainWindow::nickname_verification(QString nickname)
{
    if (nickname == nullptr)
    {
        emit type_another_nick("Nickname cannot be empty.");
        return false;
    }
    else if (correctName.exactMatch(nickname))
    {
        for(int i = 0; i < ui->onlineList->count(); ++i)
        {
            QString busy_nickname = ui->onlineList->item(i)->text();
            if (nickname == busy_nickname)
            {
                emit type_another_nick("Nickname is busy. Try another.");
                return false;
            }
        }
        return true;
    }
    else
    {
        emit type_another_nick("Use only letters and numbers (A–Z, 0-9).");
        return false;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_sendButton_clicked()
{
    QString text = chat->setData() + " " + user->nickname + ":\n" + ui->textArea->text();

    ui->textArea->clear();

    emit text_sent("GENERAL", "to_all", text, QHostAddress::Broadcast);
}

void MainWindow::set_received_text(QString data)
{
    ui->chatArea->append(data);
}

void MainWindow::nick_double_clicked_process(QListWidgetItem* item)
{
    open_PM(item->text());
}

void MainWindow::received_PM_process(QString sender, QString receiver, QString text)
{
    open_PM(sender, text);
}

void MainWindow::open_PM(QString username)
{    
    if (!PMDialog.contains(qMakePair(user->nickname, username)))
    {
        PM* PMwindow = new PM(this, username, chat, user);

        PMwindow->setWindowTitle("Chat with: " + username);
        PMwindow->show();

        PMDialog.push_back(qMakePair(user->nickname, username));
    }
}

void MainWindow::open_PM(QString sender, QString message)
{
    if (sender == user->nickname)
        return;
    else if (!PMDialog.contains(qMakePair(user->nickname, sender)))
    {
        PM* PMwindow = new PM(this, sender, message, chat, user);

        PMwindow->setWindowTitle("Chat with: " + sender);
        PMwindow->show();

        PMDialog.push_back(qMakePair(user->nickname, sender));
    }
}

void MainWindow::closed_pm_process(QString username)
{
    for (int i = 0; i < PMDialog.length(); ++i)
    {
        if (PMDialog[i] == (qMakePair(user->nickname, username)))
        {
            PMDialog.removeAt(i);
            break;
        }
    }
}

void MainWindow::update_onlinelist()
{
    ui->onlineList->clear();
    for(int i=0; i<chat->peers.count(); i++)
    {
        if (!chat->peers[i].nickname.isEmpty())
        {
            ui->onlineList->addItem(chat->peers[i].nickname);
        }
    }
    ui->onlineNumText->setText("Online: " + QString::number(ui->onlineList->count()));
}

void MainWindow::on_actionAutomatic_list_update_triggered()
{
    if (ui->actionAutomatic_list_update->isChecked())
    {
        chat->timer.start(2000);
    }
    else
    {
        chat->timer.stop();
    }
}

void MainWindow::on_updateButton_clicked()
{
    if (ui->actionAutomatic_list_update->isChecked())
    {
        ui->notification->setWordWrap(true);
        ui->notification->setText("<FONT COLOR=#800000>First, turn off automatic update in the Options.</FONT>");
    }
    else
    {
        ui->notification->clear();
        ui->onlineList->clear();

        emit manual_update();
    }
}
