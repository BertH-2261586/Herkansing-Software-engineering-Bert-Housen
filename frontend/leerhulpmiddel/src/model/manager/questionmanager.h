#ifndef QUESTIONMANAGER_H
#define QUESTIONMANAGER_H


#include <QObject>
#include "../questionset.h"
#include <QList>


class QuestionManager: public QObject
{
    Q_OBJECT
public:
    QuestionManager();
    ~QuestionManager();

    QList<Questionset*> GetAllQuestionsets()
    {
        return m_questionsets;
    }
private:
    QList<Questionset*> m_questionsets;
};

#endif // QUESTIONMANAGER_H
