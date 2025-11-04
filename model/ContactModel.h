#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QStandardItemModel>
#include <QXmlStreamWriter>

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
    void addContact(Contact &contact);//Добавление контакта
    void editContact(Contact &contact, QModelIndex &index);//Редактирование контакта

    void saveModel(QString xmlPath);//Сохраняем в xml при выходе
    void loadModel(QString xmlPath);//Восстанавливаем модель из xml при входе
};

#endif // CONTACTMODEL_H
