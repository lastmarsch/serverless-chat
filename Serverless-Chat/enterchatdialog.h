#ifndef ENTERCHATDIALOG_H
#define ENTERCHATDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QMessageBox>

namespace Ui {
class EnterChatDialog;
}

/*!
* \brief Класс, обеспечивающий авторизацию пользователя.
*
* Запрещает использовать логин пользователя, который уже есть в сети,
* а также закрывать окно, пока не авторизован.
*/

class EnterChatDialog : public QDialog
{
    Q_OBJECT

public:    
    explicit EnterChatDialog(QWidget *parent = nullptr);
    ~EnterChatDialog();

signals:    
    void set_nickname(QString nickname);
    void rejected_enterdialog();

public slots:    
    void end_work();

    /*! \fn void on_loginButton_clicked()
    *   \brief По сигналу clicked() выдает сигнал set_nickname(QString) для того,
    *   чтобы в MainWindow произошла проверка никнейма на валидность.
    */
    void on_loginButton_clicked();

    /*! \fn void warning_message(QString warning)
    *   \brief Получает по сигналу type_another_nick(QString) из MainWindow предупреждение
    *   о неверном вводе никнейма \a warning.
    *   \param warning Текст предупреждения.
    */
    void warning_message(QString warning);

private:
    Ui::EnterChatDialog *ui;

    void reject();
};

#endif // ENTERCHATDIALOG_H
