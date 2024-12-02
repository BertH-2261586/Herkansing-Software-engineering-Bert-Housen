
#ifndef QUESTIONSETCONTROLLER_H
#define QUESTIONSETCONTROLLER_H


#include "../model/questionset.h"

class QuestionsetController
{
public:
    QuestionsetController(Questionset* qset);

    void addQuestion(Question* newQuestion);

    void addSubset(QString name);

    void changeName(QString newName);

private:
    Questionset* m_questionset;

};

#endif // QUESTIONSETCONTROLLER_H
