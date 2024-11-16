#ifndef EXAMINATIONMANAGER_H
#define EXAMINATIONMANAGER_H

#include "../examination.h"

class ExaminationManager : public QObject
{
    Q_OBJECT

public:
    ExaminationManager() : m_examination(Examination()) {}

signals:
    void firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions);
    void questionLoadedModel(shared_ptr<Question> question);

public slots:
    void examinationStarted(const QString questionSetPath);
    void nextQuestion();

private:
    Examination m_examination;

};

#endif 