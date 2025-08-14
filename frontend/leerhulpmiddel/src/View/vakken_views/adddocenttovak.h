
#ifndef ADDDOCENTTOVAK_H
#define ADDDOCENTTOVAK_H


#include <QObject>
#include <QDialog>
#include <QSettings>
#include <QLayout>

#include "../../model/manager/vakkenmanager.h"
#include "../../Controller/vakkenmanagercontroller.h"


class AddDocentToVak : public QDialog
{
    Q_OBJECT
public:
    AddDocentToVak(VakkenManager* vakkenManager, Vak* vak);

private:
    QDialog* m_parent;
    Vak* m_vak;
    VakkenManager* m_vakkenManager;
    VakkenManagerController m_vmanagerController;

};

#endif // ADDDOCENTTOVAK_H
