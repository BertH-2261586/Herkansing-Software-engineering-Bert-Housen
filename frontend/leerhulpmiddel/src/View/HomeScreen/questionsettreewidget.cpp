#include "questionsettreewidget.h"
#include "mylineedit.h"
#include <QDebug>

QuestionsetTreeWidget::QuestionsetTreeWidget(Questionset* questionset, int indentation, QuestionsetTreeWidget* questionsetWidgetParent) :
    m_questionset(questionset), m_questionsetWidgetParent(questionsetWidgetParent), m_indentation(indentation)
{
    m_underlyingTree = MakeVragenTree(m_questionset->GetLooseQuestions(), m_questionset->GetSubSets(), indentation + 1);           //1 laag van de boom maken

    connect(this, &QuestionsetTreeWidget::addSubset, questionset, &Questionset::addSubSet);
    connect(questionset, &Questionset::displayNewSubSet, this, &QuestionsetTreeWidget::insertSubset);


    if (questionsetWidgetParent != nullptr)
    {
        QVBoxLayout* container = MakeExpandableVragensetButton(m_questionset->GetName(), indentation, m_underlyingTree);
        setLayout(container);   //de knop om de boom te openen samen met de boom packagen en layouts goed zetten
    }
    else
    {
        QVBoxLayout* container = new QVBoxLayout();
        container->setContentsMargins(0, 0, 0, 0);
        container->setSpacing(0);

        m_underlyingTree->show();
        container->addWidget(m_underlyingTree);

        setLayout(container);
    }
}



// gaat de boom opbouwen van de vragen die onder een vragenset horen, kan recursief werken normaal om zo subdelen te maken onder een vragenset
QWidget* QuestionsetTreeWidget::MakeVragenTree(QList<Question*> looseQuestions, QList<Questionset *> subSets, int indentation)
{
    m_underlyingTreeContainer = new QVBoxLayout();
    m_underlyingTreeContainer->setAlignment(Qt::AlignTop);
    m_underlyingTreeContainer->setContentsMargins(0, 0, 0, 0);
    m_underlyingTreeContainer->setSpacing(0);

    for (int i = 0; i < subSets.length(); i++)
    {
        QWidget* subPart = new QuestionsetTreeWidget(subSets[i], indentation, this);

        m_underlyingTreeContainer->addWidget(subPart);
    }
    AddLooseVragenToTree(m_underlyingTreeContainer, looseQuestions, indentation);

    QWidget* outputTree = new QWidget();
    outputTree->setLayout(m_underlyingTreeContainer);
    outputTree->hide();

    return outputTree;
}


//maak een "folder" button die uitklapt met alles wat er onder zit
//TODO deze mogelijks in zijn eigen klasse zetten
QVBoxLayout* QuestionsetTreeWidget::MakeExpandableVragensetButton(QString name, int indentation, QWidget* treeToHide)
{
    QWidget* questionsetWidget = new QWidget();
    questionsetWidget->setStyleSheet(
        "QWidget:hover {"
        "   background-color: #4d4d4d;"
        "}"
//        "QWidget:!hover {"
//        "    background-color: transparent;"
//        "}"
    );


    QHBoxLayout* questionsetContainer = new QHBoxLayout();
    questionsetContainer->setContentsMargins(0, 0, 0, 0);
    questionsetContainer->setSpacing(0);

    QVBoxLayout* outputContainer = new QVBoxLayout();
    outputContainer->setContentsMargins(0, 0, 0, 0);
    outputContainer->setSpacing(0);

    QPushButton* questionsetButton = new QPushButton(name, questionsetWidget);
    questionsetButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QObject::connect(questionsetButton, &QPushButton::clicked, treeToHide, [=]() {
            treeToHide->setVisible(!treeToHide->isVisible());
        }, Qt::AutoConnection);
    questionsetButton->setStyleSheet(QString(           //TODO Button flickert, fix zoeken
        "QPushButton { "
        "   color: #000000;"
        "   background-color: transparent;"
        "   border: none; "
        "   border-radius: 0px;"
        "   padding: 5px 30px 5px %1px;"
        "   text-align: left;"
        "}"
        ).arg((10 * indentation) + 30));
    //TODO nog een icon toevoegen dat dit expandable is en zo een pijltje ook


    QPushButton* addToQuestionset = new QPushButton();           //hier een plus icoon aan geven en styling doen
    addToQuestionset->setIcon(addToQuestionset->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    addToQuestionset->setStyleSheet(
        "QPushButton { "
        "   color: #000000;"
        "   background-color: transparent;"
        "   border: none; "
        "   border-radius: 2px;"
        "   padding: 0px 0px 0px 0px;"
        "   text-align: center;"
        "}"
    );
    addToQuestionset->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QMenu* addToQuestionsetMenu = new QMenu(this);
    QAction* addSubsetAction = addToQuestionsetMenu->addAction("Voeg subfolder toe");      //TODO deze strings aanpassen naar wat gepast is
    QAction* addQuestionAction = addToQuestionsetMenu->addAction("Voeg vraag toe");
    addToQuestionsetMenu->setStyleSheet("background-color: #4d4d4d;");

    connect(addToQuestionset, &QPushButton::clicked, addToQuestionsetMenu, [=]{
       addToQuestionsetMenu->popup(addToQuestionset->mapToGlobal(QPoint(0, addToQuestionset->height())));
    });

    connect(addSubsetAction, &QAction::triggered, this, &QuestionsetTreeWidget::CreateNewQuestionset);

    questionsetContainer->addWidget(questionsetButton, 15);
    questionsetContainer->addWidget(addToQuestionset, 1);

    questionsetWidget->setLayout(questionsetContainer);

    outputContainer->addWidget(questionsetWidget);
    outputContainer->addWidget(treeToHide);

    return outputContainer;
}

void QuestionsetTreeWidget::AddLooseVragenToTree(QVBoxLayout* container, QList<Question*> list, int indentation)
{
    for (int i = 0; i < list.length(); i++)
    {
        QString vraagName = list[i]->getName();

        QPushButton* looseVraagButton = new QPushButton(vraagName);
        looseVraagButton->setCheckable(true);

        looseVraagButton->setStyleSheet(QString(                            //de styling van een subdeel, werk met padding zodat de button background er goed uitziet
                                "QPushButton { "
                                "   color: #000000;"
                                "   border: none;"
                                "   border-radius: 0px;"
                                "   padding: 5px 30px 5px %1px;"
                                "   text-align: left;"
                                "}"
                                "QPushButton:hover {"
                                "   background-color: #4d4d4d;"
                                "}"
//                                "QPushButton:checked {"
//                                "    background-color: #4d4d4d;"
//                                "    border: 1px solid #FFFFFF;"
//                                "color: white;"
//                                "}"
                                ).arg((10 * indentation) + 30));

        QObject::connect(looseVraagButton, &QPushButton::clicked, this, [this, vraagName]() {
            sendDisplayQuestionSignal(new QLabel(vraagName));

            });

        container->addWidget(looseVraagButton);
    }
}


//mogelijks scuffed, het was did of een pointer naar het homescreen meegeven aan iedere instance van questionsetWidget
void QuestionsetTreeWidget::sendDisplayQuestionSignal(QWidget* toBeDisplayed)
{
    if (m_questionsetWidgetParent != nullptr)
    {
        m_questionsetWidgetParent->sendDisplayQuestionSignal(toBeDisplayed);
    }
    else
    {
        emit Display(toBeDisplayed);
    }
}

void QuestionsetTreeWidget::CreateNewQuestionset()
{
    MyLineEdit* textfield = new MyLineEdit();

    if (m_underlyingTree->isHidden())
    {
        m_underlyingTree->show();
    }

    m_underlyingTreeContainer->insertWidget(0, textfield, 0);
    textfield->setFocus();

    connect(textfield, &MyLineEdit::lostFocus, m_underlyingTreeContainer, [=]{            //zodat de invulbox er niet blijft staan als je eruit klikt en hij is leeg
            m_underlyingTreeContainer->removeWidget(textfield);
            textfield->setParent(nullptr);
            textfield->deleteLater();
        }, Qt::AutoConnection);

    connect(textfield, &MyLineEdit::returnPressed, m_underlyingTreeContainer, [=]{
            QString input = textfield->text();

            if (input != "")
            {
                emit addSubset(input);

                m_underlyingTreeContainer->removeWidget(textfield);
                textfield->setParent(nullptr);
                textfield->deleteLater();


            }
            else
            {
                m_underlyingTreeContainer->removeWidget(textfield);
                textfield->setParent(nullptr);
                textfield->deleteLater();
            }

        }, Qt::AutoConnection);
}


void QuestionsetTreeWidget::insertSubset(Questionset* newSubSet, int index)
{
    m_underlyingTreeContainer->insertWidget(index, new QuestionsetTreeWidget(newSubSet, m_indentation + 1, this), 0);
}
void QuestionsetTreeWidget::insertQuestion(Question* newQuestion)
{

}



