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
#include "../../Controller/questionmanagercontroller.h"

class HomeScreen;

class QuestionsetBrowser: public QWidget
{
public:
    QuestionsetBrowser(QList<Questionset*> allQuestionsets, QuestionManagerController* questionManagerController, HomeScreen* parent = nullptr);
    void CreateNewQuestionset();
private:
    QGraphicsView* GenerateQuestionsetTabs();
    QPushButton* GenerateCreateNewQuestionsetButton();


    void SetPermaDisplayTab(QGraphicsProxyWidget* proxy, QuestionsetWidget* questionsetWidget);

    void renderAgain();

    QHBoxLayout* m_container;
    HomeScreen* m_parent;
    QList<Questionset*> m_allQuestionsets;

    QWidget* m_displayedTree;
    QLayout* m_displayedTreeContainer;
    QList<QuestionsetWidget*> m_allQuestionsetWidgets;
    QuestionManagerController* m_questionManagerController;
    QGraphicsView* m_questionsetsTab;
};

#endif // QUESTIONSETBROWSER_H
