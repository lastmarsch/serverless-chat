#include "peer.h"

peer::peer()
{
    host = QHostAddress(QHostAddress::LocalHost);
    onlineTime = QTime::currentTime();
}

peer::peer(QString name, QHostAddress hostaddr) : nickname(name),
    host(hostaddr)
{
    onlineTime = QTime::currentTime();
}
