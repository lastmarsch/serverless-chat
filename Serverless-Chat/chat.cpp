#include "chat.h"

Chat::Chat(QObject *parent, peer* User) :
    QObject(parent)
{
    QSettings *settings = new QSettings("settings.ini",QSettings::IniFormat);
    port = static_cast<quint16>(settings->value("settings/port", 1252).toUInt());
    ip = settings->value("settings/IP", "127.0.0.1").toString();

    IPs = QNetworkInterface::allAddresses();
    timer.start(2000);
    user = User;

    qDebug() << port << ip;

    udpSocket = new QUdpSocket(this);   
    udpSocket->bind(ip, port, QUdpSocket::ReuseAddressHint);


    connect(&timer, SIGNAL(timeout()), this, SLOT(is_online()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(this, SIGNAL(update_list()), parent, SLOT(update_onlinelist()));
    connect(parent, SIGNAL(text_sent(QString, QString, QString, QHostAddress)),
            this, SLOT(send(QString, QString, QString, QHostAddress)));
    connect(this, SIGNAL(data_received(QString)),
            parent, SLOT(set_received_text(QString)));
    connect(parent, SIGNAL(manual_update()), this, SLOT(is_online()));
    connect(this, SIGNAL(data_received_pm(QString, QString, QString)),
            parent, SLOT(received_PM_process(QString, QString, QString)));
}

void Chat::send(QString type, QString receiver, QString text, QHostAddress to)
{
    QByteArray datagram;
    QString data = type + ":" + name + ":" + receiver + ":" + text;

    datagram = data.toUtf8();

    udpSocket->writeDatagram(datagram,datagram.size(), to, port);
}

void Chat::read()
{
    QByteArray datagram;
    QHostAddress address;
    QString data = "";
    while(udpSocket->hasPendingDatagrams())
    {

        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));

        udpSocket->readDatagram(datagram.data(), datagram.size(), &address);

        data += QString(datagram);
    }
    data_process(data, address);
}

void Chat::data_process(QString data, QHostAddress address)
{
    QStringList packet = data.split(":");

    if (packet[0] == "PM")
    {
        if (packet[1] == name || packet[2] == name)
        {
            QString text = packet[3];
            for (int i = 4; i < packet.size(); i++)
            {
                text += ":" + packet[i];
            }
            emit data_received_pm(packet[1], packet[2], text);
        }
    }
    else if (packet[0] == "GENERAL")
    {
        QString text = packet[3];
        for (int i = 4; i < packet.size(); i++)
        {
            text += ":" + packet[i];
        }
        emit data_received(text);
    }
    else if (packet[0] == "I'M ONLINE")
    {
        int existPos = -1;
        for(int i=0; i<peers.count(); i++)
        {
            if(peers[i].nickname == packet[1])
            {
                existPos = i;
                break;
            }
        }
        if (existPos != -1)
        {
            peers[existPos].onlineTime = QTime::currentTime();
        }
        else
        {
            peer newpeer(packet[1], address);
            peers.append(newpeer);
            emit update_list();
        }
    }
}

void Chat::is_online()
{
    send("I'M ONLINE", name, "broadcast", QHostAddress::Broadcast);
    for (int i = 0; i < peers.size(); i++)
    {
        if (peers[i].onlineTime.secsTo(QTime::currentTime()) > 5)
        {
            peers.removeAt(i);
            emit update_list();
        }
    }
}

QString Chat::setData()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString format = "[dd.MM.yyyy|hh:mm:ss]";
    return dt.toString(format);
}

void Chat::setName(QString nickname)
{
    name = nickname;
}

