
#include "vakkenmanagercontroller.h"

VakkenManagerController::VakkenManagerController(VakkenManager* vakkenmanager) : m_vakkenManager(vakkenmanager)
{

}


void VakkenManagerController::AddVak(QString naam)
{
    m_vakkenManager->addVak(naam);
}

void VakkenManagerController::AddExamToVak(int vakID, QString fileName)
{
    m_vakkenManager->AddExamToVak(vakID, fileName);
}

void VakkenManagerController::AddDocentToVak(int docentID, int vakID, QString titel)
{
    m_vakkenManager->addDocentToVak(docentID, vakID, titel);
}


void VakkenManagerController::removeDocent(int docentID, int vakID)
{
    m_vakkenManager->removeDocent(docentID, vakID);
}
