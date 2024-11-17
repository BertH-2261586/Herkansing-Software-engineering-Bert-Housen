
#include "questionsetbrowser.h"
#include "QuestionsetBrowserWidgets/questionsettreewidget.h"
#include "homescreen.h"
#include "QuestionsetBrowserWidgets/questionsetwidget.h"
#include <QDebug>

#include <QGraphicsProxyWidget>
#include <QTimer>

QuestionsetBrowser::QuestionsetBrowser(QList<Questionset *> allQuestionsets, QuestionManagerController* questionManagerController, HomeScreen* parent) 
    : m_parent(parent), m_allQuestionsets(allQuestionsets), m_questionManagerController(questionManagerController)
{
    m_container = new QHBoxLayout(this);
    m_container->setSpacing(0);
    m_container->setContentsMargins(0, 0, 0, 0);

    //m_container->addWidget(GenerateCreateNewQuestionsetButton());

    m_displayedTree = new QWidget();        //empty widget init zodat her niet crashed
    m_displayedTreeContainer = new QVBoxLayout();
    m_displayedTreeContainer->setContentsMargins(0, 0, 0, 0);
    m_displayedTreeContainer->addWidget(m_displayedTree);

    m_container->addLayout(m_displayedTreeContainer);
    m_container->addWidget(GenerateQuestionsetTabs());

    setLayout(m_container);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

//Gaat de tabladen met zijwaartse text genereren in een QgraphicsView
QGraphicsView* QuestionsetBrowser::GenerateQuestionsetTabs()
{
    QGraphicsScene* scene = new QGraphicsScene();

    QGraphicsView* view = new QGraphicsView(scene);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    view->setFrameShape(QFrame::NoFrame);


    int questionsetWidgetPosition = 0;

    for (int i = 0; i < m_allQuestionsets.length(); i++)
    {
        QuestionsetWidget* questionsetWidget = new QuestionsetWidget(m_allQuestionsets[i], m_parent, m_questionManagerController);


        m_allQuestionsetWidgets.append(questionsetWidget);

        QGraphicsProxyWidget* widgetProxy = scene->addWidget(questionsetWidget);

        widgetProxy->setCacheMode(QGraphicsItem::DeviceCoordinateCache);            //zorgt ervoor dat er geen render problemen zijn

        connect(questionsetWidget, &QuestionsetWidget::clicked, m_allQuestionsets[i], [=]{
            QuestionsetTreeWidget* tree = questionsetWidget->getUnderlyingTree();
            tree->setStyleSheet(QString("background-color: %1; border-top: 1px solid #000000;").arg(m_allQuestionsets[i]->GetColor().name()));

            setPermaDisplayTab(widgetProxy, questionsetWidget);

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

    return view;
}


//Gaat een tablad van een questionset permanent vanboven zetten en de alle rest op hun originele z-coordinaat
void QuestionsetBrowser::setPermaDisplayTab(QGraphicsProxyWidget* proxy, QuestionsetWidget* questionsetWidget)
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
    QPushButton* button = new QPushButton("nieuwe vragenset maken");

    //TODO nog styling toevoegen
    connect(button, &QPushButton::clicked, this, &QuestionsetBrowser::CreateNewQuestionset);


    return button;
}




//TODO dit aanpassen zodat je een vragenset kunt toevoegen en verder ook de namen moeten aanpassen van alles
void QuestionsetBrowser::CreateNewQuestionset()
{
    FocusOutLineEdit* textfield = new FocusOutLineEdit();

    m_container->insertWidget(1, textfield, 0);
    textfield->setFocus();

    connect(textfield, &FocusOutLineEdit::lostFocus, m_container, [=]{            //zodat de invulbox er niet blijft staan als je eruit klikt en hij is leeg
        m_container->removeWidget(textfield);
        textfield->setParent(nullptr);
        textfield->deleteLater();
    }, Qt::AutoConnection);

    connect(textfield, &FocusOutLineEdit::returnPressed, m_container, [=]{
            QString input = textfield->text();

            qDebug() << input;

            if (input != "" && doesQuestionsetExist(input) == false)
            {
                m_container->removeWidget(textfield);
                textfield->setParent(nullptr);
                textfield->deleteLater();


                QuestionsetTreeWidget* tempVragensetWidget = new QuestionsetTreeWidget(new Questionset(input, {}, {}), m_questionManagerController);     //TODO er voor zorgen dat de memory veilig werdt behandelt
                if (m_parent != nullptr)
                {
                    connect(tempVragensetWidget, &QuestionsetTreeWidget::Display, m_parent, &HomeScreen::DisplayWidget);
                }
                m_container->insertWidget(1, tempVragensetWidget, 0);
            }
            else if (input == "")
            {
                m_container->removeWidget(textfield);
                textfield->setParent(nullptr);
                textfield->deleteLater();
            }
            else
            {
                //TODO wat doen als questionset al bestaat
            }

    }, Qt::AutoConnection);
}


bool QuestionsetBrowser::doesQuestionsetExist(QString name)
{
    for (int i = 0; i < m_allQuestionsets.length(); i++)
    {
        if (name == m_allQuestionsets[i]->GetName())
        {
            return true;
        }
    }
    return false;
}

