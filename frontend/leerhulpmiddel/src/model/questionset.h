
#ifndef QUESTIONSET_H
#define QUESTIONSET_H


#include "qvariant.h"
#include <QString>
#include <QHash>
#include <QMetaType>
#include <QColor>
#include "Questions/Question.h"

class Questionset : public QObject
{
    Q_OBJECT

public:
    Questionset(QString name, QList<Question*> looseQuestions, QList<Questionset*> subSets, QColor color = {0, 0, 0});
    ~Questionset();

    QString GetName(){return m_name;};
    QList<Question*> GetLooseQuestions(){
        return m_looseQuestions;
    };
    QList<Questionset*> GetSubSets(){
        return m_subSets;
    };
    QColor GetColor(){return m_color;}

    void addQuestion(Question* question, QString subsetName = "");
private:
    QString m_name;

    QList<Question*> m_looseQuestions;
    QList<Questionset*> m_subSets;

    QColor m_color;

public slots:
    void addComponent(Questionset* subset);
    void addComponent(Question* question);
};

#endif // QUESTIONSET_H
