#include "ContactModel.h"
#include <QStandardItem>
ContactModel::ContactModel(QObject *parent) : QStandardItemModel(parent)
{

}

void ContactModel::addContact(Contact &cont)
{
    if(cont.isEmpty())
        return;

    QStandardItem *contact = new QStandardItem(cont.name + " " + cont.surname);
    contact->setEditable(false);

    contact->appendRow(new QStandardItem(cont.name));
    contact->appendRow(new QStandardItem(cont.surname));
    contact->appendRow(new QStandardItem(cont.phone));
    if(!cont.email.isEmpty())
        contact->appendRow(new QStandardItem(cont.email));
    if(!cont.note.isEmpty())
        contact->appendRow(new QStandardItem(cont.note));

    this->appendRow(contact);
}
