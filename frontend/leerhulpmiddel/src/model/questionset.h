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
    Questionset(QString name, QList<Question*> looseQuestions, QList<Questionset*> subSets, QColor color = {255, 0, 0}, Questionset* parent=nullptr);
    ~Questionset();

    // Getters
    QString GetName() const { return m_name; }
    QList<Question*> GetLooseQuestions() const { return m_looseQuestions; }
    QList<Questionset*> GetSubSets() const { return m_subSets; }
    QColor GetColor() const { return m_color; }


    void SetName(QString newName);

    void addQuestion(Question* question);
    void addSubSet(QString name);
    bool hasQuestion() const;

private:
    QString m_name;

    QList<Question*> m_looseQuestions;
    QList<Questionset*> m_subSets;

    QColor m_color;

    Questionset* m_parent;

    inline void addParent(Questionset* parent) { m_parent = parent; }

    QString getPath();

signals:
    void displayNewSubSet(Questionset* subSet, int index);
    void displayNewQuestion(Question* newQuestion, int index);

    void changed();
};

#endif // QUESTIONSET_H
