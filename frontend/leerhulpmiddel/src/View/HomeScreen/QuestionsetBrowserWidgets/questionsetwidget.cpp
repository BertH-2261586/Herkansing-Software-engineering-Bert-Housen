#include "questionsetwidget.h"
#include <QHBoxLayout>
#include "../homescreen.h"

QuestionsetWidget::QuestionsetWidget(Questionset* questionset, HomeScreen* homeScreenParent, QuestionManagerController* questionManagerController, bool topBorder, QWidget* parent) :
    m_questionset(questionset), QWidget(parent), m_homeScreenParent(homeScreenParent)
{
    setMouseTracking(true);     //nodig om mouse enter en leave events te detecteren

    QHBoxLayout* container = new QHBoxLayout();
    container->setContentsMargins(0, 0, 0, 0);
    m_label = new EditableLabel(questionset->GetName());

    connect(m_label, &EditableLabel::textChanged, this, [=](QString newText){
        questionManagerController->changeQuestionsetName(newText, m_questionset);
    });


    QString styleSheet(
        "   background-color: %1;"
        "   color: #000000;"
        );
    if (topBorder)
    {
        styleSheet += "border-left: 1px solid #000000;";
    }
    styleSheet += "   border-top: 1px solid #000000;"
                  "   border-right: 1px solid #000000;"
                  "   border-top-left-radius: 15px;"
                  "   border-top-right-radius: 15px;"
                  "   padding-top: 10px;"
                  "   padding-left: 20px;"
                  "   padding-bottom: 10px;"
                  "   padding-right: 20px;";

    m_label->setStyleSheet(styleSheet.arg(questionset->GetColor().name()));

    container->addWidget(m_label);

    setLayout(container);
    setStyleSheet(
        "background-color: transparent;"
    );

    m_underlyingTree = new QuestionsetTreeWidget(questionset);

    connect(m_underlyingTree, &QuestionsetTreeWidget::Display, m_homeScreenParent, &HomeScreen::DisplayWidget);
}

// zal normaal niet uit zichzelf verwijderd worden
QuestionsetWidget::~QuestionsetWidget()
{
    delete m_underlyingTree;
}
