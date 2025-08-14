
#ifndef SCORE_H
#define SCORE_H


#include <QString>
#include <QJsonObject>

class Score
{
public:
    Score(QJsonObject jsonData);

    int getPercent(){return m_percent;};
    QString getNaam(){return m_student;};

private:
    QString m_student;
    int m_percent;

};

#endif // SCORE_H
