#ifndef QUESTIONMANAGERCONTROLLER_H
#define QUESTIONMANAGERCONTROLLER_H

#include "../model/manager/questionmanager.h"


class QuestionManagerController
{
public:
    QuestionManagerController(QuestionManager& questionManager);


private:
    QuestionManager& m_questionManager;
};

#endif // QUESTIONMANAGERCONTROLLER_H
