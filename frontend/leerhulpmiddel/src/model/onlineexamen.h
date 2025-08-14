
#ifndef ONLINEEXAMEN_H
#define ONLINEEXAMEN_H

#include <QString>
#include <QList>
#include <QTime>

#include "Questions/Question.h"
#include "score.h"


class OnlineExamen
{
public:
    OnlineExamen(QJsonObject jsonData);

    static bool doesAlreadyExist(QString naam);
    static void saveExamen(QList<QString> questionPaths, QString naam, QTime tijdslimiet);

    int getId(){return m_id;};
    QString getNaam(){return m_naam;};
    QList<std::shared_ptr<Question>> getQuestions(){return m_questions;};
    QTime getTijd(){return m_tijd;};
    QList<Score> getScores(){return m_scores;};
    int getAverage();

private:
    QList<Score> m_scores;
    int m_id;
    QString m_naam;
    QList<std::shared_ptr<Question>> m_questions;
    QTime m_tijd;

};

#endif // ONLINEEXAMEN_H
