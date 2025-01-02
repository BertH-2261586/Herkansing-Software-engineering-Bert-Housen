#ifndef EXAMINATIONMANAGER_H
#define EXAMINATIONMANAGER_H

#include "../examination.h"

class ExaminationManager : public QObject
{
    Q_OBJECT

public:
    ExaminationManager() : m_examination(Examination()) {}
    bool finishedExamination() const { return m_examination.finishedExamination(); }

signals:
    void firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions);
    void questionLoadedModel(shared_ptr<Question> question);
    void wrongQuestionsLoadedModel(shared_ptr<Question> question, int newSize);
    void sendExaminationData(QMap<QString, QString> data);

public slots:
    void examinationStarted(const QString questionSetPath);
    void examinationStartedL(const QList<QString> questionSetPath);
    void nextQuestion(bool showWrong);
    void answeredWrong(bool timeout) { m_examination.addWrongCurrentQuestion(timeout); }
    void getExaminationData();

private:
    Examination m_examination;
};

#endif 