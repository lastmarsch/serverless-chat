#ifndef PM_H
#define PM_H

#include <QDialog>
#include "chat.h"
#include "peer.h"

namespace Ui {
class PM;
}

/*!
* \brief Класс окна приватного чата.
*
* Пересылает по сигналу введенный текст в экземпляр класса Chat, принимает датаграммы типа "PM",
* выбирает подходящую для текущих собеседников и выводит на экран.
*/

class PM : public QDialog
{
    Q_OBJECT

public:
    explicit PM(QWidget *parent = nullptr, QString destination_name = "", Chat* chat = nullptr, peer* user = nullptr);
    explicit PM(QWidget *parent = nullptr, QString destination_name = "", QString text = "", Chat* chat = nullptr, peer* user = nullptr);
    ~PM();

signals:
    void text_sent_pm(QString type, QString receiver, QString text, QHostAddress to);

    void pm_closed(QString receiver);

private slots:

    /*! \fn void on_sendButton_clicked()
    *   \brief По клику по кнопке Send передает сообщение типа "PM" в Chat для отправки.
    */
    void on_sendButton_clicked();

    void set_received_text_pm(QString sender, QString receiver, QString data);

private:
    Ui::PM *ui;

    QString talker; /*!< Никнейм собеседника. */

    Chat* ch;

    peer* us;

    void reject();
};

#endif // PM_H
