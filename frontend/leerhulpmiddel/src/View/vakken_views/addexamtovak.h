
#ifndef ADDEXAMTOVAK_H
#define ADDEXAMTOVAK_H


#include <QObject>
#include <QDialog>
#include <QString>

#include "../../model/manager/vakkenmanager.h"
#include "../../Controller/vakkenmanagercontroller.h"

class AddExamToVak : public QDialog
{
    Q_OBJECT
public:
    AddExamToVak(VakkenManager* vakkenManager, Vak *vak);

private:
    Vak* m_vak;
    VakkenManager* m_vakkenManager;
    VakkenManagerController m_vmanagerController;
};

#endif // ADDEXAMTOVAK_H
