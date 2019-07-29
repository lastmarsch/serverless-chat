#ifndef CHAT_H
#define CHAT_H

#include <QtCore>
#include <QtNetwork>
#include "peer.h"

/*!
* \brief Класс, который обеспечивает коммуникацию между пирами.
*
* Формирует и отсылает датаграммы для общего и приватного чата,
* отслеживает онлайн пользователей.
*/

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = nullptr, peer* User = nullptr);
    ~Chat() {}

    void setName(QString name);

    QString setData();

    QList<peer>peers; /*!< Список пользователей чата. */

    QTimer timer;

signals:
    void data_received(QString data);

    void data_received_pm(QString sender, QString receiver, QString data);

    void update_list();

public slots:

    /*! \fn void is_online()
    *   \brief Запускается по сигналу timeout() каждые 2 секунды.
    *   Отправляет сообщение типа "I'M ONLINE" с никнеймом текущего пользователя
    *   и обновляет список, удаляя пользователей, со времени последнего пребывания в сети
    *   которых прошло более 5 секунд.
    */
    void is_online();

    /*! \fn void send(QString type, QString receiver, QString text, QHostAddress to)
    *   \brief Формирует и широковещательным сообщением отправляет датаграмму.
    *   \param type Тип сообщения ("PM", "GENERAL" или "I'M ONLINE").
    *   \param receiver Никнейм получателя.
    *   \param text Текст сообщения.
    *   \param to Адрес получателя.
    */
    void send(QString type, QString receiver, QString text, QHostAddress to);

    /*! \fn void read()
    *   \brief Читает датаграммы и переводит их в QString.
    *   Вызывает функцию сортировки сообщений void data_process(...).
    */
    void read();


private:
    QUdpSocket* udpSocket;

    peer* user;

    QString name; /*!< Имя текущего пользователя чата. */

    QList<QHostAddress> IPs; /*!< Список IP пользователей в сети. */

    quint16 port;

    QHostAddress ip;

    /*! \fn void data_process(QString data, QHostAddress address)
    *   \brief Сортирует полученные датаграммы.
    *   Если тип сообщения "PM" или "GENERAL", подает соответствующие сигналы
    *   PM или MainWindow для вывода текста на экран.
    *   Если тип сообщения "I'M ONLINE", подает сигнал в MainWindow для
    *   обновления списка пользователей.
    *   \param data Датаграмма.
    *   \param address Адрес отправителя.
    */
    void data_process(QString data, QHostAddress address);
};

#endif // CHAT_H
