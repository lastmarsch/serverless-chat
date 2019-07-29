#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QListWidgetItem>
#include "enterchatdialog.h"
#include "chat.h"
#include "pm.h"

namespace Ui {
class MainWindow;
}

/*!
* \brief Класс главного окна со списком пользователей в сети и областью общего чата.
*
* Открывает окно входа в чат, приватные окна по желанию пользователя и при получении входящего сообщения.
* Обновляет список пользователей по сигналу из экземпляра класса Chat.
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

    void close_enter_dialog();

    void manual_update();

    void text_sent(QString type, QString receiver, QString text, QHostAddress to);

    void type_another_nick(QString warning);

private slots:

    /*! \fn void closed_pm_process(QString username)
    *   \brief При закрытии окна приватного диалога удаляет его из списка открытых диалогов.
    *   \param username Имя собеседника.
    */
    void closed_pm_process(QString username);

    /*! \fn void natural_selection(QString nickname)
    *   \brief Пока никнейм не станет валидным, не записывает его в экземпляры Chat и peer.
    *   \param nickname Никнейм пользователя.
    */
    void natural_selection(QString nickname);

    /*! \fn void nick_double_clicked_process(QListWidgetItem* item)
    *   \brief По дабл клику по имени пользователя из списка вызывает окно приватного чата.
    *   \param item Объект списка.
    */
    void nick_double_clicked_process(QListWidgetItem* item);

    /*! \fn void received_PM_process(QString sender, QString receiver, QString text)
    *   \brief При полученном сообщении вызывает окно приватного чата.
    *   \param sender Отправитель.
    *   \param receiver Получатель.
    *   \param text Текст сообщения.
    */
    void received_PM_process(QString sender, QString receiver, QString text);

    /*! \fn void on_sendButton_clicked()
    *   \brief По клику по кнопке Send передает сообщение типа "GENERAL" в Chat для отправки.
    */
    void on_sendButton_clicked();

    void on_actionExit_triggered();

    /*! \fn void on_updateButton_clicked()
    *   \brief Обновляет список пользователей в сети при выключенном автоматическом обновлении.
    */
    void on_updateButton_clicked();

    void on_actionAutomatic_list_update_triggered();

    void set_received_text(QString data);

    void update_onlinelist();

private:
    Ui::MainWindow *ui;

    Chat* chat;

    EnterChatDialog* enterCh = new EnterChatDialog(this);

    peer* user;

    QRegExp correctName{"([A-Za-z0-9]+)"}; /*!< Регулярное выражение для коррекного никнейма. */

    QList<QPair<QString, QString>> PMDialog; /*!< Список открытых приватных диалогов между
                                             юзером и другими пользователями */

    /*! \fn bool nickname_verification(QString nickname)
    *   \brief Проверяет никнейм на валидность, отправляет сигнал об ошибке в EnterChatDialog.
    *   \param nickname Никнейм пользователя.
    */
    bool nickname_verification(QString nickname);

    /*! \fn void open_PM(QString username)
    *   \brief Открывает окно приватного чата, записывает пару (юзер, собеседник) в список открытых диалогов.
    *   \param username Никнейм пользователя.
    */
    void open_PM(QString username);

    /*! \fn void open_PM(QString sender, QString text)
    *   \brief При получении сообщения открывает окно приватного чата,
    *   записывает пару (юзер, собеседник) в список открытых диалогов.
    *   \param sender Никнейм пользователя.
    *   \param text Текст сообщения.
    */
    void open_PM(QString sender, QString text);
};

#endif // MAINWINDOW_H
