#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QStandardItemModel>

struct Contact{
    QString name;
    QString surname;
    QString phone;
    QString email;
    QString note;

    bool isEmpty() { return name.isEmpty() && surname.isEmpty() && phone.isEmpty() && email.isEmpty() && note.isEmpty();}
};

class ContactModel : public QStandardItemModel
{
public:
    ContactModel(QObject *parent);
    void addContact(Contact &contact);
    void deleteContact();
};

#endif // CONTACTMODEL_H
