
#ifndef VAKKENMANAGERCONTROLLER_H
#define VAKKENMANAGERCONTROLLER_H

#include "../model/manager/vakkenmanager.h"

class VakkenManagerController
{
public:
    VakkenManagerController(VakkenManager* vakkenManager);

    void AddVak(QString naam);

    void AddExamToVak(int vakID, QString fileName);
    void AddDocentToVak(int docentID, int vakID, QString titel);
    void removeDocent(int docentID, int vakID);

private:
    VakkenManager* m_vakkenManager;

};

#endif // VAKKENMANAGERCONTROLLER_H
