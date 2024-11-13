#include "questionsettreewidget.h"
#include <QDebug>

QuestionsetTreeWidget::QuestionsetTreeWidget(Questionset* questionset, QList<QString> pathTaken, int indentation, QuestionsetTreeWidget* questionsetWidgetParent) :
    m_questionset(questionset), m_pathTaken(pathTaken), m_questionsetWidgetParent(questionsetWidgetParent)
{
    m_pathTaken.append(m_questionset->GetName());            //TODO dit mogelijks weghalen, zien of het nodig is als alle connecties staan
    m_underlyingTree = MakeVragenTree(m_questionset->GetLooseQuestions(), m_questionset->GetSubSets(), indentation + 1);           //1 laag van de boom maken

    if (questionsetWidgetParent != nullptr)
    {
        setLayout(MakeExpandableVragensetButton(m_questionset->GetName(), indentation, m_underlyingTree));   //de knop om de boom te openen samen met de boom packagen en layouts goed zetten
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
    QVBoxLayout* container = new QVBoxLayout();
    container->setAlignment(Qt::AlignTop);
    container->setContentsMargins(0, 0, 0, 0);
    container->setSpacing(0);

    for (int i = 0; i < subSets.length(); i++)
    {
        QList<QString> pathTaken = m_pathTaken;
        pathTaken.append(m_questionset->GetName());
        QWidget* subPart = new QuestionsetTreeWidget(subSets[i], pathTaken, indentation, this);

        container->addWidget(subPart);
    }
    AddLooseVragenToTree(container, looseQuestions, indentation);

    QWidget* outputTree = new QWidget();
    outputTree->setLayout(container);
    outputTree->hide();

    return outputTree;
}


//maak een "folder" button die uitklapt met alles wat er onder zit
QLayout* QuestionsetTreeWidget::MakeExpandableVragensetButton(QString name, int indentation, QWidget* treeToHide)
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
    addToQuestionsetMenu->addAction("Voeg subfolder toe");      //TODO deze strings aanpassen naar wat gepast is
    addToQuestionsetMenu->addAction("Voeg vraag toe");
    addToQuestionsetMenu->setStyleSheet("background-color: #4d4d4d;");

    connect(addToQuestionset, &QPushButton::clicked, addToQuestionsetMenu, [=]{
       addToQuestionsetMenu->popup(addToQuestionset->mapToGlobal(QPoint(0, addToQuestionset->height())));
    });


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
            sendDisplayQuestionSignal(vraagName);

            });

        container->addWidget(looseVraagButton);
    }
}


//mogelijks scuffed, het was did of een pointer naar het homescreen meegeven aan iedere instance van questionsetWidget
void QuestionsetTreeWidget::sendDisplayQuestionSignal(QString tempNaam)
{
    if (m_questionsetWidgetParent != nullptr)
    {
        m_questionsetWidgetParent->sendDisplayQuestionSignal(tempNaam);
    }
    else
    {
        emit DisplayVraag(tempNaam);
    }
}


void QuestionsetTreeWidget::addSubset(Questionset* newSubset)
{

}
void QuestionsetTreeWidget::addquestion(Question* newQuestion)
{

}



