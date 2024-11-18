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

    //void addQuestion(QString questionSetName, QString subsection, Question* question);

    inline QList<Questionset*> GetAllQuestionsets() { return m_questionsets; }
private:
    QList<Questionset*> m_questionsets;

    //Questionset* getQuestionSet(QString questionSetName);
};

#endif // QUESTIONMANAGER_H
