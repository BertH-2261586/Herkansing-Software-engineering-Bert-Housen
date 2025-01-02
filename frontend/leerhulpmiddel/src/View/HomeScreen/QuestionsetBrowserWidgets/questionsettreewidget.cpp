#include "questionsettreewidget.h"
#include "questionbutton.h"
#include "questionsetbutton.h"
#include "../../focusoutlineedit.h"
#include "../../create questions/CreateQuestionView.h"

QuestionsetTreeWidget::QuestionsetTreeWidget(Questionset* questionset, int indentation, QuestionsetTreeWidget* questionsetWidgetParent) :
    m_questionset(questionset), m_questionsetWidgetParent(questionsetWidgetParent), m_indentation(indentation), m_controller(questionset)
{
    m_underlyingTree = MakeQuestionTree(m_questionset->GetLooseQuestions(), m_questionset->GetSubSets(), indentation + 1);           //1 laag van de boom maken

    connect(this, &QuestionsetTreeWidget::addSubset, questionset, &Questionset::addSubSet);
    connect(questionset, &Questionset::displayNewSubSet, this, &QuestionsetTreeWidget::insertSubset);
    connect(questionset, &Questionset::displayNewQuestion, this, &QuestionsetTreeWidget::insertQuestion);

    if (questionsetWidgetParent != nullptr)
    {
        QVBoxLayout* container = MakeExpandableQuestionsetButton(m_questionset->GetName(), indentation, m_underlyingTree);
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
QWidget* QuestionsetTreeWidget::MakeQuestionTree(QList<Question*> looseQuestions, QList<Questionset *> subSets, int indentation)
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
    AddLooseQuestionsToTree(m_underlyingTreeContainer, looseQuestions, indentation);

    QWidget* outputTree = new QWidget();
    outputTree->setLayout(m_underlyingTreeContainer);
    outputTree->hide();

    return outputTree;
}


//maak een "folder" button die uitklapt met alles wat er onder zit
//TODO deze mogelijks in zijn eigen klasse zetten
QVBoxLayout* QuestionsetTreeWidget::MakeExpandableQuestionsetButton(QString name, int indentation, QWidget* treeToHide)
{
    QWidget* questionsetWidget = new QWidget();
//    questionsetWidget->setStyleSheet(
//        "QWidget:hover {"
//        "   background-color: #4d4d4d;"
//        "}"
////        "QWidget:!hover {"
////        "    background-color: transparent;"
////        "}"
//    );


    QHBoxLayout* questionsetContainer = new QHBoxLayout();
    questionsetContainer->setContentsMargins(0, 0, 0, 0);
    questionsetContainer->setSpacing(0);

    QVBoxLayout* outputContainer = new QVBoxLayout();
    outputContainer->setContentsMargins(0, 0, 0, 0);
    outputContainer->setSpacing(0);


    QuestionsetButton* questionsetButton = new QuestionsetButton(m_questionset, m_indentation, this);
    connect(questionsetButton, &QuestionsetButton::clicked, treeToHide, [=](){
        treeToHide->setVisible(!treeToHide->isVisible());
        }, Qt::AutoConnection);
//    QPushButton* questionsetButton = new QPushButton(name, questionsetWidget);
//    questionsetButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    QObject::connect(questionsetButton, &QPushButton::clicked, treeToHide, [=]() {
//            treeToHide->setVisible(!treeToHide->isVisible());
//        }, Qt::AutoConnection);
//    questionsetButton->setStyleSheet(QString(           //TODO Button flickert, fix zoeken
//        "QPushButton { "
//        "   color: #000000;"
//        "   background-color: transparent;"
//        "   border: none; "
//        "   border-radius: 0px;"
//        "   padding: 5px 30px 5px %1px;"
//        "   text-align: left;"
//        "}"
//        ).arg((10 * indentation) + 30));
//    //TODO nog een icon toevoegen dat dit expandable is en zo een pijltje ook


    //QPushButton* addToQuestionset = GenerateMenuButton();

//    QMenu* addToQuestionsetMenu = new QMenu(this);
//    QAction* addSubsetAction = addToQuestionsetMenu->addAction("Voeg subfolder toe");      //TODO deze strings aanpassen naar wat gepast is
//    QAction* addQuestionAction = addToQuestionsetMenu->addAction("Voeg vraag toe");
//    addToQuestionsetMenu->setStyleSheet("background-color: #4d4d4d;");

//    connect(addToQuestionset, &QPushButton::clicked, addToQuestionsetMenu, [=]{
//       addToQuestionsetMenu->popup(addToQuestionset->mapToGlobal(QPoint(0, addToQuestionset->height())));
//    });

//    connect(addSubsetAction, &QAction::triggered, this, &QuestionsetTreeWidget::CreateNewQuestionset);
//    connect(addQuestionAction, &QAction::triggered, this, [=] {
//        sendDisplayQuestionSignal(new CreateQuestionView(m_questionManagerController, this));
//    });

    questionsetContainer->addWidget(questionsetButton, 15);
    //questionsetContainer->addWidget(addToQuestionset, 1);

    questionsetWidget->setLayout(questionsetContainer);

    outputContainer->addWidget(questionsetWidget);
    outputContainer->addWidget(treeToHide);

    return outputContainer;
}

void QuestionsetTreeWidget::AddLooseQuestionsToTree(QVBoxLayout* container, QList<Question*> list, int indentation)
{
    for (int i = 0; i < list.length(); i++)
    {
        QuestionButton* loosQuestionButton = new QuestionButton(list[i], indentation);

        QObject::connect(loosQuestionButton, &QuestionButton::clicked, this, [this, list, i]() {
            sendDisplayQuestionSignal(new QLabel(list[i]->getName()));        //TODO nog de goede widgets versturen
        });

        container->addWidget(loosQuestionButton);
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
    FocusOutLineEdit* textfield = new FocusOutLineEdit();

    if (m_underlyingTree->isHidden())
    {
        m_underlyingTree->show();
    }

    m_underlyingTreeContainer->insertWidget(0, textfield, 0);
    textfield->setFocus();

    connect(textfield, &FocusOutLineEdit::lostFocus, m_underlyingTreeContainer, [=]{            //zodat de invulbox er niet blijft staan als je eruit klikt en hij is leeg
            QString input = textfield->text();

            if (input != "")
            {
                m_underlyingTreeContainer->removeWidget(textfield);
                textfield->deleteLater();

                m_controller.addSubset(input);
            }
            else
            {
                m_underlyingTreeContainer->removeWidget(textfield);
                textfield->deleteLater();
            }
        }, Qt::AutoConnection);

    connect(textfield, &FocusOutLineEdit::escPressed, m_underlyingTreeContainer, [=]{
            m_underlyingTreeContainer->removeWidget(textfield);
            textfield->setParent(nullptr);
            textfield->deleteLater();
        }, Qt::AutoConnection);
//    connect(textfield, &FocusOutLineEdit::returnPressed, m_underlyingTreeContainer, [=]{


//        }, Qt::AutoConnection);
}


void QuestionsetTreeWidget::insertSubset(Questionset* newSubSet, int index)
{
    m_underlyingTreeContainer->insertWidget(index, new QuestionsetTreeWidget(newSubSet, m_indentation + 1, this), 0);
}
void QuestionsetTreeWidget::insertQuestion(Question* newQuestion, int index)
{
    QuestionButton* newQButton = new QuestionButton(newQuestion, m_indentation + 1, this);
    m_underlyingTreeContainer->insertWidget(index, newQButton, 0);
    QObject::connect(newQButton, &QuestionButton::clicked, this, [this, newQuestion]() {
        sendDisplayQuestionSignal(new QLabel(newQuestion->getName()));        //TODO nog de goede widgets versturen
        });
    emit Display(new QWidget());
}