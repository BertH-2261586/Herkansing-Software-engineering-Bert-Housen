#ifndef QUESTIONMANAGERCONTROLLER_H
#define QUESTIONMANAGERCONTROLLER_H

#include "../model/questionmanager.h"

#include <QString>

class QuestionManagerController
{
public:
    QuestionManagerController(QuestionManager& questionManager);

    inline void addQuestion(QString questionSetName, QString subsection, Question* question) { m_questionManager.addQuestion(questionSetName, subsection, question); }
    inline void openQuestionSet(QString questionSetName) {}
private:
    QuestionManager& m_questionManager;
};

#endif // QUESTIONMANAGERCONTROLLER_H
