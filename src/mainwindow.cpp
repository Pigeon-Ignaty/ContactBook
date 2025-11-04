#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRegularExpression"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setSizes({500,100});
    ui->splitter->setCollapsible(0,false);
    ui->splitter->setCollapsible(1,false);

    m_model = new ContactModel(this);
    QString xmlPath = QCoreApplication::applicationDirPath() + "/data.xml";
    m_model->loadModel(xmlPath);
    ui->contactList->setModel(m_model);

    connect(ui->m_nameEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);
    connect(ui->m_surnameEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);
    connect(ui->m_phoneEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);
    connect(ui->m_emailEdit, &QLineEdit::textChanged, this, &MainWindow::updateSaveButtonState);

    connect(ui->contactList, &QListView::clicked, this, &MainWindow::onContactClicked);
    connect(ui->contactList->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
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
    if(result){
        auto contact = m_addDialog->getContact();
        m_model->addContact(contact);
        QModelIndex index = m_model->index(m_model->rowCount() - 1, 0);

        ui->contactList->setCurrentIndex(index);
        ui->contactList->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void MainWindow::on_m_saveBtn_clicked()
{
    Contact contact;
    contact.name = ui->m_nameEdit->text();
    contact.surname = ui->m_surnameEdit->text();
    contact.phone = ui->m_phoneEdit->text();
    if(!ui->m_emailEdit->text().isEmpty())
        contact.email = ui->m_emailEdit->text();
    if(!ui->m_noteEdit->text().isEmpty())
        contact.note = ui->m_noteEdit->text();

    m_model->addContact(contact);
    QModelIndex index = m_model->index(m_model->rowCount() - 1, 0);

    ui->contactList->setCurrentIndex(index);
    ui->contactList->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void MainWindow::updateSaveButtonState()
{
    bool nameOk = !ui->m_nameEdit->text().isEmpty();
    bool surnameOk = !ui->m_surnameEdit->text().isEmpty();
    bool phoneOk = QRegularExpression("^\\d+$").match(ui->m_phoneEdit->text()).hasMatch();
    QString email = ui->m_emailEdit->text();
    bool emailOk = email.isEmpty() || QRegularExpression("^[^@]+@[^@]+$").match(email).hasMatch();

    ui->m_saveBtn->setEnabled(nameOk && surnameOk && phoneOk && emailOk);
}

void MainWindow::onContactClicked(const QModelIndex &index)
{
//    if(!index.isValid())
//        return;

//    auto model = dynamic_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(index.model()));
//    if(!model)
//        return;

//    QStandardItem *item = model->itemFromIndex(index);
//    if(!item)
//        return;

//    int rowCount = item->rowCount();
//    QVector<QLineEdit*> widgets = {ui->m_nameEdit, ui->m_surnameEdit, ui->m_phoneEdit, ui->m_emailEdit, ui->m_noteEdit};
//    for (int i = 0; i < rowCount; i++) {
//        if(i < rowCount && item->child(i,0)){
//            widgets[i]->setText(item->child(i, 0)->text());
//        }
//        else {
//            widgets[i]->clear();
//        }
//    }

}

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // Если выделение снято — очищаем все поля и выходим
    if (selected.indexes().isEmpty()) {
        ui->m_nameEdit->clear();
        ui->m_surnameEdit->clear();
        ui->m_phoneEdit->clear();
        ui->m_emailEdit->clear();
        ui->m_noteEdit->clear();
        showEditWidgets(false);
        return; // ← важно, чтобы дальше не шло
    }

    // Берём первый выделенный элемент
    QModelIndex index = selected.indexes().first();
    auto* model = qobject_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(index.model()));
    if (!model) return;

    QStandardItem* item = model->itemFromIndex(index);
    if (!item) return;

    QVector<QLineEdit*> widgets = {
        ui->m_nameEdit,
        ui->m_surnameEdit,
        ui->m_phoneEdit,
        ui->m_emailEdit,
        ui->m_noteEdit
    };
    showEditWidgets(true);

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

    m_model->saveModel(xmlPath);
    QMainWindow::closeEvent(event);
}
