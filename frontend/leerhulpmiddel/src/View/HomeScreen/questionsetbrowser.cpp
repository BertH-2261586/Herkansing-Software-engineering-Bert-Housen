
#include "questionsetbrowser.h"
#include "QuestionsetBrowserWidgets/questionsettreewidget.h"
#include "homescreen.h"
#include "QuestionsetBrowserWidgets/questionsetwidget.h"
#include <QDebug>

#include <QGraphicsProxyWidget>
#include <QTimer>

QuestionsetBrowser::QuestionsetBrowser(QList<Questionset *> allQuestionsets, QuestionManagerController* questionManagerController, HomeScreen* parent)
    : m_parent(parent), m_allQuestionsets(), m_questionManagerController(questionManagerController)
{
    m_container = new QHBoxLayout(this);
    m_container->setSpacing(0);
    m_container->setContentsMargins(0, 0, 0, 0);

    //m_container->addWidget(GenerateCreateNewQuestionsetButton());

    m_displayedTree = new QWidget();        //empty widget init zodat her niet crashed
    m_displayedTreeContainer = new QVBoxLayout();
    m_displayedTreeContainer->setContentsMargins(0, 0, 0, 0);
    m_displayedTreeContainer->addWidget(m_displayedTree);

    connect(questionManagerController->getManager(), &QuestionManager::changed, this, &QuestionsetBrowser::renderAgain);

    m_container->addLayout(m_displayedTreeContainer);

    m_questionsetsTab = GenerateQuestionsetTabs();
    m_container->addWidget(m_questionsetsTab);

    setLayout(m_container);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

//Gaat de tabladen met zijwaartse text genereren in een QgraphicsView
QGraphicsView* QuestionsetBrowser::GenerateQuestionsetTabs()
{
    m_allQuestionsets = m_questionManagerController->getAllQuestionsets();

    QGraphicsScene* scene = new QGraphicsScene();

    QGraphicsView* view = new QGraphicsView(scene);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    view->setFrameShape(QFrame::NoFrame);

    int questionsetWidgetPosition = 0;

    for (int i = 0; i < m_allQuestionsets.length(); i++)
    {
        QuestionsetWidget* questionsetWidget = new QuestionsetWidget(m_allQuestionsets[i], m_parent, m_questionManagerController, i != 0);


        m_allQuestionsetWidgets.append(questionsetWidget);

        QGraphicsProxyWidget* widgetProxy = scene->addWidget(questionsetWidget);

        widgetProxy->setCacheMode(QGraphicsItem::DeviceCoordinateCache);            //zorgt ervoor dat er geen render problemen zijn

        connect(questionsetWidget, &QuestionsetWidget::clicked, m_allQuestionsets[i], [=]{
            QuestionsetTreeWidget* tree = questionsetWidget->getUnderlyingTree();
            tree->setStyleSheet(QString("background-color: %1;").arg(m_allQuestionsets[i]->GetColor().name()));

            SetPermaDisplayTab(widgetProxy, questionsetWidget);

            m_displayedTreeContainer->replaceWidget(m_displayedTree, tree);
            m_displayedTree->hide();
            tree->show();

            m_displayedTree = tree;
        });

        //Lift label omhoog als er met de muis overheen gaat
        connect(questionsetWidget, &QuestionsetWidget::hoverEnter, widgetProxy, [this, widgetProxy, questionsetWidget]{
            if (questionsetWidget->getPermaDisplay() == false)
            {
                widgetProxy->setZValue(1000);
            }
            widgetProxy->update();
        });
        //terug naar beneden als de muis weggaat
        connect(questionsetWidget, &QuestionsetWidget::hoverLeave, widgetProxy, [this, widgetProxy, i, questionsetWidget]{
            if (questionsetWidget->getPermaDisplay() == false)
            {
                widgetProxy->setZValue(m_allQuestionsets.length() - i);
            }
            widgetProxy->update();
        });

        widgetProxy->setPos(0, questionsetWidgetPosition);
        widgetProxy->setZValue(m_allQuestionsets.length() - i);
        widgetProxy->setRotation(90);

        questionsetWidgetPosition += questionsetWidget->width() * 3 / 4;            //hier wordt de spacing tussen tabladen gezet
    }

    QGraphicsProxyWidget* newQuestionsetButton = scene->addWidget(GenerateCreateNewQuestionsetButton());

    newQuestionsetButton->setPos(0, questionsetWidgetPosition + 50);
    newQuestionsetButton->setRotation(90);

    return view;
}


//Gaat een tablad van een questionset permanent vanboven zetten en de alle rest op hun originele z-coordinaat
void QuestionsetBrowser::SetPermaDisplayTab(QGraphicsProxyWidget* proxy, QuestionsetWidget* questionsetWidget)
{
    questionsetWidget->setPermaDisplay(true);
    proxy->setZValue(500);

    for (int i = 0; i < m_allQuestionsetWidgets.length(); i++)
    {
        if (m_allQuestionsetWidgets[i] != questionsetWidget)
        {
            m_allQuestionsetWidgets[i]->setPermaDisplay(false);
            emit m_allQuestionsetWidgets[i]->hoverLeave();
        }
    }
    proxy->update();
}

QPushButton* QuestionsetBrowser::GenerateCreateNewQuestionsetButton()
{
    QPushButton* button = new QPushButton("+");
    button->setStyleSheet("   background-color: #afafaf; color: #000000;"
                          "   border: 1px solid #000000;"
                          "   border-radius: 15px;"
                          "   padding-top: 10px;"
                          "   padding-left: 20px;"
                          "   padding-bottom: 10px;"
                          "   padding-right: 20px;");

    button->setContentsMargins(0, 0, 0, 0);

    button->setAttribute(Qt::WA_TranslucentBackground);

    connect(button, &QPushButton::clicked, this, &QuestionsetBrowser::CreateNewQuestionset);

    return button;
}




//TODO dit aanpassen zodat je een vragenset kunt toevoegen en verder ook de namen moeten aanpassen van alles
void QuestionsetBrowser::CreateNewQuestionset()
{
    m_questionManagerController->addQuestionset();
}

void QuestionsetBrowser::renderAgain()
{
    QGraphicsView* temp = GenerateQuestionsetTabs();

    m_questionsetsTab->hide();
    m_container->replaceWidget(m_questionsetsTab, temp);

    m_questionsetsTab->deleteLater();
    m_questionsetsTab = temp;
}

