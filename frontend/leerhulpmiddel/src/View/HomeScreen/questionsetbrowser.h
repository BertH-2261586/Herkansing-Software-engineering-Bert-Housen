#ifndef QUESTIONSETBROWSER_H
#define QUESTIONSETBROWSER_H

#include <QPushButton>
#include "qwidget.h"
#include <QHBoxLayout>
#include "../focusoutlineedit.h"
#include <QList>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "../../model/questionset.h"
#include "QuestionsetBrowserWidgets/questionsetwidget.h"

class HomeScreen;

class QuestionsetBrowser: public QWidget
{
public:
    QuestionsetBrowser(QList<Questionset*> allQuestionsets, HomeScreen* parent = nullptr);

private:
    QGraphicsView* GenerateQuestionsetTabs();
    QPushButton* GenerateCreateNewQuestionsetButton();
    void CreateNewQuestionset();
    bool doesQuestionsetExist(QString name);

    void setPermaDisplayTab(QGraphicsProxyWidget* proxy, QuestionsetWidget* questionsetWidget);

    QHBoxLayout* m_container;
    HomeScreen* m_parent;
    QList<Questionset*> m_allQuestionsets;

    QWidget* m_displayedTree;
    QLayout* m_displayedTreeContainer;
    QList<QuestionsetWidget*> m_allQuestionsetWidgets;
};

#endif // QUESTIONSETBROWSER_H
