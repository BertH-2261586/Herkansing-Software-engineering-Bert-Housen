#ifndef VRAGENSETWIDGET_H
#define VRAGENSETWIDGET_H

#include "qwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QMenu>
#include "../../model/questionset.h"


class QuestionsetTreeWidget: public QWidget
{
    Q_OBJECT


public:
    QuestionsetTreeWidget(Questionset *questionset, QList<QString> pathTaken = QList<QString>(), int indentation = 0, QuestionsetTreeWidget* questionsetWidgetParent = nullptr);


private:
    QWidget* MakeVragenTree(QList<Question *> looseQuestions, QList<Questionset*> subSets, int indentation);
    void AddLooseVragenToTree(QVBoxLayout* container, QList<Question*> list, int indentation);
    QLayout *MakeExpandableVragensetButton(QString name, int indentation, QWidget* treeToHide);

    void sendDisplayQuestionSignal(QString tempNaam);

    QWidget* m_underlyingTree;
    QuestionsetTreeWidget* m_questionsetWidgetParent;
    Questionset* m_questionset;
    QList<QString> m_pathTaken;



signals:
    void DisplayVraag(QString tempNaam);

public slots:
    void addSubset(Questionset* newSubset);
    void addquestion(Question* newQuestion);
};

#endif // VRAGENSETWIDGET_H
