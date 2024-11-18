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
    Questionset(QString name, QList<Question*> looseQuestions, QList<Questionset*> subSets, QColor color = {255, 0, 0});
    ~Questionset();

    // Getters
    QString GetName() const { return m_name; }
    QList<Question*> GetLooseQuestions() const { return m_looseQuestions; }
    QList<Questionset*> GetSubSets() const { return m_subSets; }
    QColor GetColor() const { return m_color; }

    void addQuestion(Question* question, QString subsetName = "");
private:
    QString m_name;

    QList<Question*> m_looseQuestions;
    QList<Questionset*> m_subSets;

    QColor m_color;

signals:
    void displayNewSubSet(Questionset* subSet, int index);
    void displayNewQuestion(Question* newQuestion, int index);

public slots:
    void addSubSet(QString name);
    void addComponent(Question* question);
};

#endif // QUESTIONSET_H
