
#include "questionsetcontroller.h"

QuestionsetController::QuestionsetController(Questionset* qset) : m_questionset(qset)
{

}

void QuestionsetController::addQuestion(Question* newQuestion)
{
    m_questionset->addQuestion(newQuestion);
}

void QuestionsetController::addSubset(QString name)
{
    m_questionset->addSubSet(name);
}

void QuestionsetController::changeName(QString newName)
{
    m_questionset->SetName(newName);
}
