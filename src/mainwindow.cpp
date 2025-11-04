#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRegularExpression"
#include <QDebug>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setSizes({500,100});
    ui->splitter->setCollapsible(0,false);
    ui->splitter->setCollapsible(1,false);

    //Восстанавливаем модель из xml
    m_model = new ContactModel(this);
    QString xmlPath = QCoreApplication::applicationDirPath() + "/data.xml";
    m_model->loadModel(xmlPath);
    ui->contactList->setModel(m_model);

    //Проверка корректности значений в полях контакта
    connect(ui->m_nameEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);
    connect(ui->m_surnameEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);
    connect(ui->m_phoneEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);
    connect(ui->m_emailEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);

    connect(ui->contactList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectionChanged);
    showEditWidgets(false);
    m_addDialog = new AddContactDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_addBtn_clicked()
{
    ui->m_saveBtn->setEnabled(false);
    bool result = m_addDialog->exec();
    if(result){//Если нажали сохранить
        auto contact = m_addDialog->getContact();//Получаем контакт
        m_model->addContact(contact);
        QModelIndex index = m_model->index(m_model->rowCount() - 1, 0);//Выделяем последний элемент

        ui->contactList->setCurrentIndex(index);
        ui->contactList->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void MainWindow::on_m_saveBtn_clicked()
{
    auto currentIndex = ui->contactList->currentIndex();
    if(!currentIndex.isValid())
        return;

    //Собираем значение полей в структуру
    Contact contact;
    contact.name = ui->m_nameEdit->text();
    contact.surname = ui->m_surnameEdit->text();
    contact.phone = ui->m_phoneEdit->text();
    contact.email = ui->m_emailEdit->text();
    contact.note = ui->m_noteEdit->text();
    //Меняем поля модели
    m_model->editContact(contact, currentIndex);
}

void MainWindow::updateSaveButtonState()
{
    //Проверка введённых данных
    bool nameOk = !ui->m_nameEdit->text().isEmpty();
    bool surnameOk = !ui->m_surnameEdit->text().isEmpty();
    bool phoneOk = QRegularExpression("^\\d+$").match(ui->m_phoneEdit->text()).hasMatch();
    QString email = ui->m_emailEdit->text();
    bool emailOk = email.isEmpty() || QRegularExpression("^[^@]+@[^@]+$").match(email).hasMatch();

    ui->m_saveBtn->setEnabled(nameOk && surnameOk && phoneOk && emailOk);
}

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Если выделение снято, то очищаем и скрываем виджеты
    if (selected.indexes().isEmpty()) {
        ui->m_nameEdit->clear();
        ui->m_surnameEdit->clear();
        ui->m_phoneEdit->clear();
        ui->m_emailEdit->clear();
        ui->m_noteEdit->clear();
        showEditWidgets(false);
        ui->m_deleteBtn->setEnabled(false);
        return;
    }

    //Берём первый выделенный элемент
    QModelIndex index = selected.indexes().first();
    auto model = qobject_cast<QStandardItemModel*>(ui->contactList->model());
    if (!model) return;

    QStandardItem* item = model->itemFromIndex(index);
    if (!item) return;

    ui->m_deleteBtn->setEnabled(true);
    QVector<QLineEdit*> widgets = {
        ui->m_nameEdit,
        ui->m_surnameEdit,
        ui->m_phoneEdit,
        ui->m_emailEdit,
        ui->m_noteEdit
    };
    showEditWidgets(true);
    //Заполняем значениями в виджеты
    int rowCount = item->rowCount();
    for (int i = 0; i < widgets.size(); ++i) {
        if (i < rowCount && item->child(i, 0))
            widgets[i]->setText(item->child(i, 0)->text());
        else
            widgets[i]->clear();
    }
}

void MainWindow::showEditWidgets(bool show)
{
    //Показываем или скрываем боковые виджеты
    if(show){
        ui->m_nameEdit->show();
        ui->m_surnameEdit->show();
        ui->m_phoneEdit->show();
        ui->m_emailEdit->show();
        ui->m_noteEdit->show();

        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->label_5->show();
        ui->label->show();
    }
    else{
        ui->m_nameEdit->hide();
        ui->m_surnameEdit->hide();
        ui->m_phoneEdit->hide();
        ui->m_emailEdit->hide();
        ui->m_noteEdit->hide();

        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->label->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString xmlPath = QCoreApplication::applicationDirPath() + "/data.xml";
    //Сохраняем контакты при выходе
    m_model->saveModel(xmlPath);
    QMainWindow::closeEvent(event);
}

void MainWindow::on_m_deleteBtn_clicked()
{
    //Получаем текущий выбранный индекс
    QModelIndex currentIndex = ui->contactList->currentIndex();
    if(!currentIndex.isValid())
        return;
    //Получаем модель из индекса и снимаем const
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(currentIndex.model()));
    if(!model)
        return;

    //Получаем родителя текущего элемента
    QStandardItem *parentItem = model->itemFromIndex(currentIndex.parent());

    QStandardItem *item = model->itemFromIndex(currentIndex);
    if (!item)
        return;

    auto reply = QMessageBox::question(this, "Удалить контакт?",
                          "Вы уверены, что хотите удалить контакт " + item->text(),
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return;

    if(parentItem) {
        parentItem->removeRow(currentIndex.row());
    }
    else{
        model->removeRow(currentIndex.row());
    }
}

void MainWindow::on_action_triggered()
{
        QMessageBox::about(this, "О программе",
                           "Список контактов v1.0\n"
                           "Простое приложение для хранения списка контактов.\n");
}
