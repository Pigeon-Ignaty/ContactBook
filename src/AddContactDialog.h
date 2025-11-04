#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>
#include "ContactModel.h"
namespace Ui {
class AddContactDialog;
}

class AddContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactDialog(QWidget *parent = nullptr);
    ~AddContactDialog();
    Contact getContact();
private:
    Ui::AddContactDialog *ui;
protected:
    void showEvent(QShowEvent *event) override;
    //void hideEvent(QHideEvent *event) override;
public slots:
    void updateSaveButtonState();
};

#endif // ADDCONTACTDIALOG_H
