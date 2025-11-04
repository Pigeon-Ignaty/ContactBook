#include "ContactModel.h"
#include <QStandardItem>
#include <QXmlStreamWriter>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
ContactModel::ContactModel(QObject *parent) : QStandardItemModel(parent)
{

}

void ContactModel::addContact(Contact &cont)
{
    if(cont.isEmpty())
        return;
    //Создаём модель с именем + фамилией
    QStandardItem *contact = new QStandardItem(cont.name + " " + cont.surname);
    contact->setEditable(false);
    //Добавляем все поля, даже если пустые
    contact->appendRow(new QStandardItem(cont.name));
    contact->appendRow(new QStandardItem(cont.surname));
    contact->appendRow(new QStandardItem(cont.phone));
    contact->appendRow(new QStandardItem(cont.email));
    contact->appendRow(new QStandardItem(cont.note));

    this->appendRow(contact);
}

void ContactModel::editContact(Contact &contact, QModelIndex &index)
{
    if (!index.isValid())
        return;

    //Снимаем const, чтобы получить изменяемый указатель
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(index.model()));
    if (!model)
        return;

    QStandardItem *item = model->itemFromIndex(index);
    if (!item)
        return;
    //Меняем название контакта
    item->setText(contact.name + " " + contact.surname);

    while (item->rowCount() < 5)
        item->appendRow(new QStandardItem);
    //Обновляем все значения полей
    if(item->rowCount() >= 5 ){
        item->child(0)->setText(contact.name);
        item->child(1)->setText(contact.surname);
        item->child(2)->setText(contact.phone);
        item->child(3)->setText(contact.email);
        item->child(4)->setText(contact.note);
    }

    emit model->dataChanged(index, index);
}

void ContactModel::saveModel(QString xmlPath)
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Cannot open file for writing");
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("contacts"); //Создаём корень контакты

    for (int i = 0; i < rowCount(); i++) {
        QStandardItem *contactItem = item(i);//Получаем сам контакт
        if (!contactItem)
            continue;

        xml.writeStartElement("contact"); //Создаём тег контакт

        //Основное название контакта
        xml.writeTextElement("title", contactItem->text());

        //Сохраняем все поля в виде тегов и значений, даже если они нулевые
        for (int j = 0; j < 5; j++) {
            QStandardItem *child = contactItem->child(j);
            QString tag;
            switch (j)
            {
            case 0: tag = "name"; break;
            case 1: tag = "surname"; break;
            case 2: tag = "phone"; break;
            case 3: tag = "email"; break;
            case 4: tag = "note"; break;
            }
            xml.writeTextElement(tag, child ? child->text() : "");//Записываем либо из модели, либо пустую строку
        }
        xml.writeEndElement(); //Закрываем тег контакт
    }

    xml.writeEndElement(); //Закрываем тег контакты
    xml.writeEndDocument();

    file.close();
}


void ContactModel::loadModel(QString xmlPath)
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QXmlStreamReader xml(&file);
    clear();
    //Читаем корневой элемент контакты
    if (xml.readNextStartElement() && xml.name() == "contacts") {
        while (xml.readNextStartElement()) {
            if (xml.name() == "contact") {//Находим контакт
                QStandardItem *contactItem = new QStandardItem;
                contactItem->setEditable(false);

                QStringList tags = {"name", "surname", "phone", "email", "note"};

                while (xml.readNextStartElement()) {
                    QString tagName = xml.name().toString();

                    if (tagName == "title") {//Основное название контакта
                        contactItem->setText(xml.readElementText());
                    }
                    else if (tags.contains(tagName)) {//Остальные данные контакта добавляем как дочерние элементы
                        contactItem->appendRow(new QStandardItem(xml.readElementText()));
                    }
                    else{
                        xml.skipCurrentElement();
                    }
                }

                appendRow(contactItem); //добавляем контакт в модель
            }
            else{
                xml.skipCurrentElement();
            }
        }
    }
    else{
        xml.skipCurrentElement();
    }

    file.close();
}

