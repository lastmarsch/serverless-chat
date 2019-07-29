#ifndef PEER_H
#define PEER_H

#include <QtCore>
#include <QtNetwork>

/*!
* \brief Класс, описывающий пользователя чата.
*
* Содержит никнейм, адрес и время последнего пребывания в сети.
*/

class peer
{
public:
    peer();
    peer(QString name, QHostAddress hostaddr);

    QString nickname;   /*!< Никнейм пользователя. */

    QHostAddress host;  /*!< IP пользователя. */

    QTime onlineTime;   /*!< Время последнего онлайна. */
};

#endif // PEER_H
