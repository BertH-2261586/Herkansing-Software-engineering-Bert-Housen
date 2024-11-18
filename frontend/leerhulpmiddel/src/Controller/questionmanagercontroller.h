#ifndef QUESTIONMANAGERCONTROLLER_H
#define QUESTIONMANAGERCONTROLLER_H

#include "../model/manager/questionmanager.h"

#include <QString>

class QuestionManagerController
{
public:
    QuestionManagerController(QuestionManager& questionManager);

    //inline void addQuestion(QString questionSetName, QString subsection, Question* question) { m_questionManager.addQuestion(questionSetName, subsection, question); }
    inline void openQuestionSet(QString questionSetName) {}
    inline QList<Questionset*> getAllQuestionsets() const { return m_questionManager.GetAllQuestionsets(); }
private:
    QuestionManager& m_questionManager;
};

#endif // QUESTIONMANAGERCONTROLLER_H
