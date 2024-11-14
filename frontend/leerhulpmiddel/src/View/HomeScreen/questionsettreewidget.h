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
    QuestionsetTreeWidget(Questionset *questionset, int indentation = 0, QuestionsetTreeWidget* questionsetWidgetParent = nullptr);


private:
    QWidget* MakeVragenTree(QList<Question *> looseQuestions, QList<Questionset*> subSets, int indentation);
    void AddLooseVragenToTree(QVBoxLayout* container, QList<Question*> list, int indentation);
    QVBoxLayout *MakeExpandableVragensetButton(QString name, int indentation, QWidget* treeToHide);

    void CreateNewQuestionset();
    bool doesQuestionsetExist(QString name);

    void sendDisplayQuestionSignal(QWidget* toBeDisplayed);

    QWidget* m_underlyingTree;
    QVBoxLayout* m_underlyingTreeContainer;
    QuestionsetTreeWidget* m_questionsetWidgetParent;
    Questionset* m_questionset;
    int m_indentation;


signals:
    void Display(QWidget* displayWidget);

    void addSubset(QString name);

public slots:
    void insertSubset(Questionset* newSubset, int index);
    void insertQuestion(Question* newQuestion);
};

#endif // VRAGENSETWIDGET_H
