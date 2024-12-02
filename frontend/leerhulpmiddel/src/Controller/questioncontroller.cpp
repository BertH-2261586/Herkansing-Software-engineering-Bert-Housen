
#include "questioncontroller.h"

QuestionController::QuestionController(Question* question) : m_question(question)
{
}


void QuestionController::changeName(QString newName)
{
    m_question->setName(newName);
}
