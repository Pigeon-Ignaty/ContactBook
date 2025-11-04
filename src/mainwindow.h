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
    void on_m_addBtn_clicked();//Слот добавления

    void on_m_saveBtn_clicked();
    void updateSaveButtonState(); //Слот проверки всех введённых данных
    void onContactClicked(const QModelIndex &index);
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
private:
    Ui::MainWindow *ui;
    ContactModel *m_model = nullptr;
    bool isValid = false; //Переменная разрешающая сохранение, если данные валидные
    void showEditWidgets(bool show);
    AddContactDialog *m_addDialog;

};

#endif // MAINWINDOW_H
