#include "AddContactDialog.h"
#include "ui_AddContactDialog.h"
#include <QRegularExpression>
AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);

    //Проверка корректности значений в полях контакта
    connect(ui->m_nameEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);
    connect(ui->m_surnameEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);
    connect(ui->m_phoneEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);
    connect(ui->m_emailEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);

    connect(ui->m_cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->m_save, &QPushButton::clicked, this, &QDialog::accept);

    this->setStyleSheet(R"(
        QDialog{
            background-color: #f2f4f7;
        }
        QLabel{
            color: #2b2b2b;
            font-weight: bold;
        }
        QLineEdit{
            background-color: #ffffff;
            border: 1px solid #a8b0ba;
            border-radius: 6px;
            padding: 4px;
        }
        QLineEdit:focus{
            border: 1px solid #0078d7;
            background-color: #f0f8ff;
        }
        QPushButton{
            background-color: #0078d7;
            color: white;
            border-radius: 6px;
            padding: 5px 10px;
            font-weight: 500;
        }
        QPushButton:hover{
            background-color: #005fa3;
        }
        QPushButton:disabled{
            background-color: #c0c0c0;
            color: #666666;
        }
    )");
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

Contact AddContactDialog::getContact()
{
    Contact contact;
    contact.name = ui->m_nameEdit->text();
    contact.surname = ui->m_surnameEdit->text();
    contact.phone = ui->m_phoneEdit->text();
    contact.email = ui->m_emailEdit->text();
    contact.note = ui->m_noteEdit->text();

    return contact;
}

void AddContactDialog::showEvent(QShowEvent *event)
{
    ui->m_nameEdit->clear();
    ui->m_nameEdit->setFocus();
    ui->m_surnameEdit->clear();
    ui->m_phoneEdit->clear();
    ui->m_emailEdit->clear();
    ui->m_noteEdit->clear();

    QDialog::showEvent(event);
}

void AddContactDialog::updateSaveButtonState()
{
    bool nameOk = !ui->m_nameEdit->text().isEmpty();
    bool surnameOk = !ui->m_surnameEdit->text().isEmpty();
    bool phoneOk = QRegularExpression("^\\d+$").match(ui->m_phoneEdit->text()).hasMatch();//Если все цифры
    QString email = ui->m_emailEdit->text();
    bool emailOk = email.isEmpty() || QRegularExpression("^[^@]+@[^@]+$").match(email).hasMatch();//Действительно, если пусто или *@*

    ui->m_save->setEnabled(nameOk && surnameOk && phoneOk && emailOk);
}
