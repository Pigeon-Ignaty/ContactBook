#include "AddContactDialog.h"
#include "ui_AddContactDialog.h"
#include <QRegularExpression>
AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);

    connect(ui->m_nameEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);
    connect(ui->m_surnameEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);
    connect(ui->m_phoneEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);
    connect(ui->m_emailEdit, &QLineEdit::textChanged, this, &AddContactDialog::updateSaveButtonState);

    connect(ui->m_cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->m_save, &QPushButton::clicked, this, &QDialog::accept);

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
    if(!ui->m_emailEdit->text().isEmpty())
        contact.email = ui->m_emailEdit->text();
    if(!ui->m_noteEdit->text().isEmpty())
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
    bool phoneOk = QRegularExpression("^\\d+$").match(ui->m_phoneEdit->text()).hasMatch();
    QString email = ui->m_emailEdit->text();
    bool emailOk = email.isEmpty() || QRegularExpression("^[^@]+@[^@]+$").match(email).hasMatch();

    ui->m_save->setEnabled(nameOk && surnameOk && phoneOk && emailOk);
}
