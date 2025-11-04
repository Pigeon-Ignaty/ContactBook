#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ContactModel.h"
#include <QItemSelection>
#include "AddContactDialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_m_addBtn_clicked();//Слот добавления контакта
    void on_m_saveBtn_clicked();//Слот сохранение контакта после редактирования
    void on_m_deleteBtn_clicked();//Слот удаления контакта

    void updateSaveButtonState(); //Слот проверки всех введённых данных

    //Слот для снятие и установки выделения на qlistview
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_action_triggered();//О программе

private:
    Ui::MainWindow *ui;
    ContactModel *m_model = nullptr;
    AddContactDialog *m_addDialog;

    void showEditWidgets(bool show);
    void setStyle();
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
